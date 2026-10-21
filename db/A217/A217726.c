/*
 * A217726  Numbers n such that 6n is a partition number.
 * https://oeis.org/A217726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A225326(n)/6
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers n such that 6 n is a partition number (OEIS A217726) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for memoisation */
static idx_t     next_k = 1;          /* next partition index to compute */
static seq_t    *p_vals = NULL;       /* array of partition numbers p[0 ..] */
static size_t    p_cap  = 0;          /* capacity of p_vals */

static seq_t    *n_vals = NULL;       /* array of a(1), a(2), ... */
static size_t    n_cap  = 0;          /* capacity of n_vals */
static size_t    n_len  = 0;          /* number of a‑terms already found */

/* Ensure p_vals has at least ‘needed’ slots (indices 0 .. needed-1) */
static void ensure_p_cap(size_t needed)
{
    if (needed > p_cap) {
        size_t newcap = (p_cap) ? p_cap << 1 : 64;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(p_vals, newcap * sizeof(seq_t));
        if (!tmp) abort();
        p_vals = tmp;
        p_cap  = newcap;
    }
}

/* Ensure n_vals has at least ‘needed’ slots (indices 0 .. needed-1) */
static void ensure_n_cap(size_t needed)
{
    if (!n_vals) {
        n_vals = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!n_vals) abort();
        n_cap = 64;
    }
    if (needed > n_cap) {
        size_t newcap = n_cap << 1;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(n_vals, newcap * sizeof(seq_t));
        if (!tmp) abort();
        n_vals = tmp;
        n_cap  = newcap;
    }
}

/* Compute the next partition number p[next_k] and record a‑term if it is
   divisible by 6. */
static void compute_next_p(void)
{
    idx_t n = next_k;                     /* compute p[n] */
    ensure_p_cap((size_t)n + 1);

    seq_t pos = 0, neg = 0;
    int sign = 1;

    for (int k = 1; ; ++k) {
        idx_t pent1 = (idx_t)k * ((idx_t)3 * k - 1) / 2;
        idx_t pent2 = (idx_t)k * ((idx_t)3 * k + 1) / 2;

        if (pent1 > n && pent2 > n) break;

        if (pent1 <= n) {
            idx_t idx = n - pent1;
            if (sign > 0) pos += p_vals[idx];
            else          neg += p_vals[idx];
        }

        if (pent2 <= n) {
            idx_t idx = n - pent2;
            if (sign > 0) pos += p_vals[idx];
            else          neg += p_vals[idx];
        }

        sign = -sign;                     /* alternate sign per k */
    }

    seq_t val = pos;
    val -= neg;                            /* p[n] = pos - neg */

    p_vals[n] = val;

    if (val % 6 == 0) {                    /* found a(n) */
        seq_t m = val / 6;
        ensure_n_cap(n_len + 1);
        n_vals[n_len++] = m;
    }

    ++next_k;                               /* ready for next partition */
}

/* Compute a(n) for all n up to ‘target’, storing results in n_vals. */
static void compute_upto(idx_t target)
{
    if (!p_vals) {                          /* initialise partition array */
        p_vals = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!p_vals) abort();
        p_cap = 64;
        p_vals[0] = 1;                     /* p(0) = 1 */
    }

    while (n_len < target) {
        compute_next_p();
    }
}

/* The required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – offset starts at 1 */
    compute_upto(n);
    return n_vals[n - 1];
}
