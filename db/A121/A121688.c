/*
 * A121688  G.f.: Sum_{n>=0} x^n * (1+x)^(2^n).
 * https://oeis.org/A121688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(2^k, n-k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A121688.  Uses a dynamic programming accumulator that
 * builds all values up to the requested index.  The algorithm is
 * O(n^2) with a small constant factor and works for both narrow
 * integer types (up to 64 bits) and arbitrary‑precision builds. */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* compute all values up to N (inclusive) */
static void compute_upto(idx_t N)
{
    /* allocate a fresh cache, zero‑initialised */
    size_t new_size = (size_t)N + 1;
    seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_cache) exit(1);  /* out of memory */

    /* current power of two, starting at 2^0 = 1 */
    seq_t two_k = (seq_t)1;

    /* loop over k = 0 .. N */
    for (idx_t k = 0; k <= N; ++k)
    {
        /* m = 0 contributes the term 1 to a(k) itself */
        new_cache[k] += (seq_t)1;

        /* maximal m to consider for this k   (n-k) */
        idx_t max_m = N - k;

        /* if two_k is too large for a narrow type, stop the loop
         * as all further two_k values would overflow the type
         * and the binomials would be > UINT64_MAX anyway.
         * For big‑int builds this condition is never true. */
        if (sizeof(seq_t) <= 8 && k >= (sizeof(seq_t) * 8 - 1))
            break;          /* avoid left shift overflow on tiny types */

        /* compute binomials C(two_k, m) for m = 1 .. max_m */
        seq_t binom = (seq_t)1;             /* C(two_k, 0) = 1 */

        for (idx_t m = 1; m <= max_m; ++m)
        {
            /* numerator: two_k - (m-1) */
            seq_t numerator = two_k - (seq_t)(m - 1);

            /* multiply then divide, using 128‑bit if available
             * for narrow types to keep intermediate accurate. */
            if (sizeof(seq_t) <= 8)
            {
                /* 128‑bit arithmetic guarantees correctness
                 * for any 64‑bit result in the final binomial.   */
                unsigned __int128 prod = (unsigned __int128)binom *
                                         (unsigned __int128)numerator;
                binom = (seq_t)(prod / (unsigned __int128)m);
            }
            else
            {
                /* arbitrary‑precision builds rely on overloaded ops.  */
                binom = (binom * numerator) / (seq_t)m;
            }

            /* add the binomial contribution to a(k+m)   */
            new_cache[k + m] += binom;
        }

        /* prepare two_k for next iteration (k+1) */
        if (sizeof(seq_t) <= 8)
        {
            if (k >= 63) break;   /* avoid overflow on 64‑bit types */
            two_k <<= 1;
        }
        else
        {
            two_k <<= 1;           /* big‑int left shift is fine       */
        }
    }

    /* replace the old cache with the new one */
    free(cache);
    cache      = new_cache;
    cache_size = new_size;
}

/* public interface required by the framework */
seq_t a(idx_t n)
{
    if (n >= cache_size) compute_upto(n);
    return cache[n];
}
