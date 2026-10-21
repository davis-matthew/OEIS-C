/*
 * A050025  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n ≥ 4, where m = 2^(p+1) + 2 – n p is
 *            the unique integer with 2^p < (n‑1) ≤ 2^(p+1); initial values:
 *            a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 33, u16 n <= 82, u32 n <= 520, u64 n <= 4628,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute ⌊log₂(x)⌋ for x > 0.  Works on C99/C++17. */
static inline idx_t floor_log2_idx(idx_t x)
{
    /* x is guaranteed ≥ 1 in our usage. */
    return (idx_t)(63 - __builtin_clzll((unsigned long long)x));
}

/* Return the integer p such that 2^p < (n-1) ≤ 2^(p+1). */
static inline idx_t get_p(idx_t n_minus1)
{
    /* n_minus1 = n-1 ≥ 3 for n ≥ 4. */
    idx_t r = floor_log2_idx(n_minus1);
    /* If n_minus1 is an exact power of two, we need p = r-1. */
    if ((n_minus1 == ((idx_t)1 << r)))
        return r ? r - 1 : 0;  /* r is at least 1 here */
    return r;
}

seq_t a(idx_t n)
{
    /* Base cases are trivial. */
    if (n <= 3)
        return (seq_t)1;

    /* Static memoisation cache.  cache[1] … cache[max_computed] are valid. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated length (≥1)   */
    static idx_t max_computed = 0;    /* highest index with value */

    /* Grow cache if necessary. */
    if (cache_size <= n)
    {
        idx_t new_size = cache_size ? cache_size << 1 : (n + 1);
        if (new_size <= n)
            new_size = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp)
            exit(1);      /* allocation failure should never happen for test sizes */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values if this is the first call. */
    if (max_computed < 3)
    {
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        max_computed = 3;
    }

    /* Compute terms up to requested n. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i < 4)
        {
            cache[i] = (seq_t)1;   /* safety, though n≥4 here */
            continue;
        }

        /* Compute p for current i. */
        idx_t p = get_p(i - 1);

        /* Compute m = 2^(p+1) + 2 – i. */
        idx_t pow2p1 = ((idx_t)1) << (p + 1);
        idx_t m = pow2p1 + 2 - i;

        /* Safety: m must be at least 1 and < i.  If not, the sequence is
         * malformed; we still guard against out-of-bounds cache. */
        if (m < 1 || m >= i)
            m = 1;   /* fallback to a(1) – should never occur in valid data */

        cache[i] = cache[i - 1] + cache[m];
    }

    max_computed = n;
    return cache[n];
}
