/*
 * A212465  Number of 0..3 arrays of length n+3 with sum no more than 6 in any
 *          length 4 subsequence (=50% duty cycle).
 * https://oeis.org/A212465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..3 arrays of length n+3 such that every *
 *            contiguous subsequence of length 4 has sum ≤ 6. Computed by
 *            dynamic * programming over the 64 possible triples of the last
 *            three symbols
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ----------------------------------------------------------------- */
/*  Static cache for previously computed terms                       */
static seq_t *a_cache    = NULL;    /* a_cache[n] = a(n), 1‑based   */
static size_t a_cache_cap = 0;      /* capacity of a_cache          */
static idx_t a_cache_max  = 0;      /* largest n computed (≥0)      */

/* ----------------------------------------------------------------- */
/*  DP tables for the last three symbols, encoded 0..63              */
static seq_t dp[64];        /* counts for current length        */
static seq_t newdp[64];     /* temporary for next step          */

/* ----------------------------------------------------------------- */
/*  Ensure the cache contains a(n) for all n ≤ target.  Compute
 *  missing values incrementally. */
static void
ensure_cache(idx_t target)
{
    if (target <= a_cache_max)
        return;

    /* Ensure allocation of a_cache */
    size_t needed = (size_t)(target + 1);
    if (a_cache == NULL) {
        /* initial allocation */
        a_cache_cap = needed > 128 ? needed : 128;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (a_cache == NULL) abort();
        for (size_t i = 0; i < a_cache_cap; ++i)
            a_cache[i] = 0;
        /* DP for length 3: every triple occurs exactly once   */
        for (size_t i = 0; i < 64; ++i)
            dp[i] = (seq_t)1;
        a_cache_max = 0;
    } else if (needed > a_cache_cap) {
        size_t new_cap = a_cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
        for (size_t i = a_cache_cap; i < new_cap; ++i)
            a_cache[i] = 0;
        a_cache_cap = new_cap;
    }

    /* Compute missing values from (a_cache_max+1) up to target */
    for (idx_t cur = a_cache_max; cur < target; ++cur) {
        /* One transition step: DP for length (cur+3)+1 = (cur+4)   */
        for (size_t i = 0; i < 64; ++i)
            newdp[i] = 0;

        for (unsigned a0 = 0; a0 < 4; ++a0) {
            for (unsigned a1 = 0; a1 < 4; ++a1) {
                for (unsigned a2 = 0; a2 < 4; ++a2) {
                    unsigned idx  = (a0 << 4) | (a1 << 2) | a2;
                    seq_t     cnt = dp[idx];
                    if (!cnt)
                        continue;
                    unsigned sum3 = a0 + a1 + a2;
                    for (unsigned x = 0; x < 4; ++x) {
                        if (sum3 + x <= 6) {
                            unsigned nidx = (a1 << 4) | (a2 << 2) | x;
                            newdp[nidx] += cnt;
                        }
                    }
                }
            }
        }

        /* Move newdp → dp   */
        for (size_t i = 0; i < 64; ++i)
            dp[i] = newdp[i];

        /* Sum all states to obtain a(cur+1) (a(n) with n = cur+1)   */
        seq_t total = 0;
        for (size_t i = 0; i < 64; ++i)
            total += dp[i];
        a_cache[cur + 1] = total;   /* cur+1 is the n we just computed */
    }

    a_cache_max = target;
}

/* ----------------------------------------------------------------- */
/*  Public routine requested in the task description                 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1 – a(0) is undefined   */
    ensure_cache(n);
    return a_cache[n];
}
