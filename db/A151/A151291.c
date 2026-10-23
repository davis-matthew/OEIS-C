/*
 * A151291  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, 0), (1, -1), (1,
 *          0), (1, 1)}.
 * https://oeis.org/A151291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{r=⌈n/2⌉}^{n}  C(n,r) * (2*r - n + 1)/(r+1) * M_r
 * where M_r is the grand Motzkin number of length r (number of walks of
 * length r with steps {(-1,0),(0,-1),(0,0),(0,1)} staying in the first
 * quadrant).  The sum runs over all possible numbers of right steps r.
 *
 * Implementation computes M_r once with a simple DP; then each a(n)
 * is obtained by the above sum.  All arithmetic is done in the generic
 * type seq_t; idx_t is the index type supplied by the build.
 */

#include <stdbool.h>   /* for bool */

#define MAX_N 20000      /* should cover all required values (≤10 000) */

/* cache of grand Motzkin numbers M_r   */
static seq_t M_arr[MAX_N + 1];
/* cache for final sequence values a(n)   */
static seq_t a_cache[MAX_N + 1];
/* temporary arrays for computing M_r */
static seq_t cur_arr[MAX_N + 1];
static seq_t nxt_arr[MAX_N + 1];
/* track the highest n already pre‑computed */
static idx_t precomp_limit = 0;
/* initialization flags */
static bool M_initialized = false;

/* ------------------------------------------------------------------ */
/* Pre‑compute grand Motzkin numbers M_r up to 'limit'.                */
/* ------------------------------------------------------------------ */
static void ensure_M(idx_t limit)
{
    if (limit > MAX_N) return;            /* out of bounds protection */

    if (!M_initialized) {
        /* initialise for r = 0 */
        M_arr[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_N; ++i) cur_arr[i] = (seq_t)0;
        cur_arr[0] = (seq_t)1;
        M_initialized = true;
    }

    static idx_t M_max = 0;                /* highest r already computed */

    if (limit <= M_max) return;

    for (idx_t r = M_max + 1; r <= limit; ++r) {
        /* zero the next distribution */
        for (idx_t h = 0; h <= r; ++h) nxt_arr[h] = (seq_t)0;

        /* transition from cur to nxt */
        for (idx_t h = 0; h < r; ++h) {
            seq_t val = cur_arr[h];
            if (val == (seq_t)0) continue;
            /* flat step */
            nxt_arr[h] += val;
            /* up step */
            nxt_arr[h + 1] += val;
            /* down step (only if h>0) */
            if (h > 0) nxt_arr[h - 1] += val;
        }

        /* sum over all heights to obtain M_r */
        seq_t sum = (seq_t)0;
        for (idx_t h = 0; h <= r; ++h) sum += nxt_arr[h];
        M_arr[r] = sum;

        /* copy next to current for the next iteration */
        for (idx_t h = 0; h <= r; ++h) cur_arr[h] = nxt_arr[h];
    }

    M_max = limit;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute final sequence values a(n) up to 'limit'.              */
/* ------------------------------------------------------------------ */
static void ensure_a(idx_t limit)
{
    if (limit > MAX_N) return;            /* out of bounds protection */

    ensure_M(limit);                      /* we must have M_r for all r ≤ limit */

    if (precomp_limit == 0) {
        a_cache[0] = (seq_t)1;            /* a(0) = 1 */
    }

    for (idx_t n = precomp_limit + 1; n <= limit; ++n) {
        if (n == 0) continue;             /* already initialised */

        seq_t sum = (seq_t)0;
        idx_t r_start = (n + 1) / 2;

        /* compute binomial C(n, r_start) iteratively */
        seq_t binom = (seq_t)1;
        for (idx_t i = 1; i <= r_start; ++i) {
            seq_t mult = binom * (seq_t)(n - r_start + i);
            binom = mult / (seq_t)i;
        }

        for (idx_t r = r_start; r <= n; ++r) {
            seq_t factor = (seq_t)(2 * r - n + 1);
            seq_t term_num = binom * factor;
            seq_t ballot = term_num / (seq_t)(r + 1);          /* Ballot number */
            seq_t term = ballot * M_arr[r];
            sum += term;

            /* Update binomial for the next r (if any) */
            if (r < n) {
                seq_t mult = binom * (seq_t)(n - r);
                binom = mult / (seq_t)(r + 1);               /* C(n, r+1) */
            }
        }
        a_cache[n] = sum;
    }

    precomp_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build.                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
