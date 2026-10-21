/*
 * A093519  Numbers with no representation as the sum of two (not necessarily
 *          distinct) generalized pentagonal numbers.
 * https://oeis.org/A093519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth natural number that cannot be written as a sum
 * of two generalized pentagonal numbers (p_k = k(3k−1)/2 for k∈ℤ, including 0).
 *
 * Description: The routine builds a list of the non‑representable numbers.  
 * It keeps a table of representable sums up to a current limit, enlarges
 * that limit when needed, and stores the list of missing numbers.  The
 * implementation uses dynamic allocation; casting is added so the code
 * compiles under both C11 and C++17 for every permitted seq_t/idx_t type.
 * The algorithm is O(L²) for the current limit L, which is generous for
 * n≤10000 and runs well below the 2‑second limit.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool *rep = NULL;          /* rep[x] = true if x is a sum of two pentagonals */
static size_t rep_limit = 0;      /* current maximum index of rep array */

/* List of generated generalized pentagonal numbers up to rep_limit */
static seq_t *pent = NULL;
static size_t pent_len = 0, pent_cap = 0;

/* List of numbers that are NOT representable as a sum of two pentagonals */
static seq_t *nonrep = NULL;
static size_t nonrep_len = 0, nonrep_cap = 0;

static bool initialized = false;

/* Grow an array of seq_t elements */
static void grow_seq_array(seq_t **p, size_t *len, size_t *cap, size_t extra)
{
    if (*len + extra > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < *len + extra) new_cap <<= 1;
        *p = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Recompute all tables up to a given numeric limit */
static void recompute(size_t limit)
{
    /* free old data */
    free(rep);   rep = NULL;
    free(pent);  pent = NULL;  pent_len = pent_cap = 0;
    free(nonrep);nonrep = NULL;nonrep_len = nonrep_cap = 0;

    rep_limit = limit;
    rep = (bool *)calloc(limit + 1, sizeof(bool));      /* all false */

    /* Generate generalized pentagonal numbers <= limit */
    for (idx_t k = 0; ; ++k) {
        /* positive k */
        seq_t p1 = (seq_t)k * ((seq_t)3 * k - 1) / 2;
        if (p1 <= limit) {
            grow_seq_array(&pent, &pent_len, &pent_cap, 1);
            pent[pent_len++] = p1;
        }

        /* negative k – avoid duplicate 0 when k = 0 */
        if (k > 0) {
            int64_t nk = -(int64_t)k;
            seq_t p2 = (seq_t)((nk) * (3 * nk - 1) / 2);
            if (p2 <= limit) {
                grow_seq_array(&pent, &pent_len, &pent_cap, 1);
                pent[pent_len++] = p2;
            }
        }

        /* stop when both positive and negative k exceed limit */
        if (p1 > limit && k > 0 && (seq_t)((-(int64_t)k) * (3 * (-(int64_t)k) - 1) / 2) > limit)
            break;
    }

    /* Mark representable sums */
    for (size_t i = 0; i < pent_len; ++i) {
        seq_t pi = pent[i];
        for (size_t j = 0; j < pent_len; ++j) {
            seq_t sum = pi + pent[j];
            if ((size_t)sum > limit) break;   /* remaining j give larger sums */
            rep[(size_t)sum] = true;
        }
    }

    /* Build list of non-representable numbers */
    for (size_t v = 0; v <= limit; ++v) {
        if (!rep[v]) {
            grow_seq_array(&nonrep, &nonrep_len, &nonrep_cap, 1);
            nonrep[nonrep_len++] = (seq_t)v;
        }
    }
}

/* Ensure that we have at least `needed` non-representable numbers */
static void ensure_upper(idx_t needed)
{
    if (needed == 0) return;
    idx_t target = needed;                     /* number of entries required */
    while ((idx_t)nonrep_len < target) {
        size_t new_limit = (rep_limit == 0) ? 1024 * 1024 : rep_limit * 2;
        recompute(new_limit);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 for safety */

    if (!initialized) {
        /* initial limit large enough for typical queries; grows if needed */
        recompute(1024 * 1024);    /* 1 048 576 */
        initialized = true;
    }

    ensure_upper(n);
    return nonrep[n - 1];          /* 1‑based index */
}
