/*
 * A036438  Integers which can be written as m*tau(m) for some m, where tau =
 *          A000005.
 * https://oeis.org/A036438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9314, u64 n <= 9314, u128 n <= 9314,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A036438 – integers that can be written as m * tau(m)
    (tau = number of divisors).  a(1) = 1, a(2) = 4, …           */

/* Needs the helper functions provided in the libutils folder: */
#include "../utils/build_spf.h"

#include <stdlib.h>
#include <stdint.h>

/* -----------------------------------------------------------------------
   A comparison routine that works both for integral seq_t in C and for
   arbitrary‑precision seq_t in C++.  qsort needs a function of type
   int (*)(const void *, const void *) that returns negative,
   zero or positive for less than, equal to or greater than.
   ----------------------------------------------------------------------- */
static int
seq_cmp(const void *p1, const void *p2)
{
#if defined(__cplusplus)
    const seq_t &a = *static_cast<const seq_t *>(p1);
    const seq_t &b = *static_cast<const seq_t *>(p2);
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
#else
    const seq_t *pa = (const seq_t *)p1;
    const seq_t *pb = (const seq_t *)p2;
    if ( *pa < *pb ) return -1;
    if ( *pa > *pb ) return  1;
    return 0;
#endif
}

/* -----------------------------------------------------------------------
   Global cache:  array of the distinct values m*tau(m) in sorted order.
   The array is built on the first call to a().
   ----------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* -----------------------------------------------------------------------
   Return the n-th element of the sequence (1‑based index).  If n is out
   of range the function returns 0; behaviour for n < 1 is unspecified.
   ----------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Build cache on first use.  The LIMIT must be large enough to
       contain more than the requested number of terms; 200 000 is
       comfortably above 10 000 terms and well below the limits of an
       ordinary stack/heap.                                                 */
    if (cache == NULL)
    {
        const idx_t LIMIT = 200000;

        /* 1. Build smallest prime factor array.  The function allocates
           an array of LENGTH LIMIT+1, with spf[0] unused and spf[1] = 1. */
        idx_t *spf = NULL;
        build_spf(LIMIT, &spf);

        /* 2. Compute tau(m) for every m <= LIMIT.  tau[0] set to 1 for
           convenience, though it is never used. */
        uint32_t *tau = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        tau[0] = 1;
        tau[1] = 1;
        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            idx_t p   = spf[i];
            idx_t tmp = i / p;
            uint32_t exp = 1;
            while (tmp % p == 0)
            {
                ++exp;
                tmp /= p;
            }
            tau[i] = tau[tmp] * (exp + 1);
        }

        /* 3. Compute the raw values m * tau(m).  The product comfortably
           fits in seq_t for all m <= LIMIT (max ~ 2·10^7). */
        seq_t *vals = (seq_t *)malloc(LIMIT * sizeof(seq_t));
        for (idx_t m = 1; m <= LIMIT; ++m)
            vals[m - 1] = (seq_t)m * (seq_t)tau[m];

        /* 4. Sort the array, because we need the distinct, sorted
           elements for the sequence. */
        qsort(vals, (size_t)LIMIT, sizeof(seq_t), seq_cmp);

        /* 5. Count how many distinct values there are. */
        size_t uniq = 0;
        for (size_t i = 0; i < (size_t)LIMIT; ++i)
        {
            if (i == 0 || seq_cmp(&vals[i], &vals[i - 1]) != 0)
                ++uniq;
        }

        /* 6. Allocate the final cache and copy the distinct values. */
        cache = (seq_t *)malloc(uniq * sizeof(seq_t));
        size_t src = 0, dst = 0;
        while (src < (size_t)LIMIT)
        {
            cache[dst++] = vals[src];
            seq_t cur = vals[src];
            while (src < (size_t)LIMIT && seq_cmp(&vals[src], &cur) == 0)
                ++src;
        }
        cache_len = uniq;

        /* 7. Cleanup temporary arrays. */
        free(vals);
        free(tau);
        free(spf);
    }

    /* Return the requested element (1‑based).  If the index is out of
       bounds we simply return 0 – the caller's behaviour for such a
       situation is unspecified and not used in the tests. */
    if (n == 0 || (size_t)n > cache_len)
        return (seq_t)0;
    return cache[n - 1];
}
