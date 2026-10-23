/*
 * A043685  a(n) = (1/2)*(n-th number whose base-2 representation has exactly
 *          10 runs).
 * https://oeis.org/A043685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*(n-th number whose base-2 representation has
 *            exactly 10 runs)
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the binomial coefficient C(n,k) using 128‑bit arithmetic. */
static unsigned __int128 binom_uint64(idx_t n, idx_t k)
{
    if (k > n) return (unsigned __int128)0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (unsigned __int128)(n - k + i) / i;
    }
    return res;
}

/* a(n) */
seq_t a(idx_t n)
{
    /* Find the minimum number of bits L such that the cumulative number of
       binary numbers with exactly 10 runs reaches or exceeds n. */
    idx_t L = 0;
    unsigned __int128 cumulative = 0;
    while (1) {
        ++L;
        unsigned __int128 add = 0;
        if (L >= 10) {                 /* L-1 choose 9 is non‑zero only if L-1 ≥ 9 */
            add = binom_uint64(L - 1, 9);
        }
        cumulative += add;
        if (cumulative >= n) break;
    }

    /* Compute the index of the desired number among those with L bits. */
    unsigned __int128 add_current = 0;
    if (L >= 10) add_current = binom_uint64(L - 1, 9);
    unsigned __int128 cumulative_before = cumulative - add_current;
    idx_t offset = n - (idx_t)cumulative_before - 1;   /* zero‑based offset */

    /* Determine the run lengths r[0] … r[9] (1‑based runs 1…10). */
    idx_t r[10];
    idx_t remaining = L;
    idx_t k = offset;
    for (int i = 0; i < 10; ++i) {
        if (i == 9) {              /* last run gets whatever is left */
            r[i] = remaining;
            break;
        }
        idx_t runs_left = 10 - i;  /* including the current run */
        idx_t max_val = remaining - (runs_left - 1);

        if ((i & 1) == 0) {        /* odd run (1‑based odd) => minimal first */
            for (idx_t val = 1; val <= max_val; ++val) {
                idx_t rem_after = remaining - val;
                idx_t parts = runs_left - 1;
                unsigned __int128 comb = 0;
                if (parts == 0) {
                    comb = 1;
                } else {
                    comb = binom_uint64(rem_after - 1, parts - 1);
                }
                idx_t count = (idx_t)comb;
                if (k < count) {
                    r[i] = val;
                    remaining = rem_after;
                    break;
                } else {
                    k -= count;
                }
            }
        } else {                   /* even run (1‑based even) => maximal first */
            for (idx_t val = max_val; val > 0; --val) {
                idx_t rem_after = remaining - val;
                idx_t parts = runs_left - 1;
                unsigned __int128 comb = 0;
                if (parts == 0) {
                    comb = 1;
                } else {
                    comb = binom_uint64(rem_after - 1, parts - 1);
                }
                idx_t count = (idx_t)comb;
                if (k < count) {
                    r[i] = val;
                    remaining = rem_after;
                    break;
                } else {
                    k -= count;
                }
            }
        }
    }

    /* Construct the integer with the determined run lengths. */
    seq_t num = 0;
    for (int i = 0; i < 10; ++i) {
        num <<= r[i];
        if ((i & 1) == 0) {   /* odd run (1‑based odd) – fill with ones */
            seq_t ones = ((seq_t)1 << r[i]) - 1;
            num |= ones;
        }
    }

    /* The required sequence value is half of this integer. */
    return num >> 1;
}
