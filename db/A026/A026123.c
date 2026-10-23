/*
 * A026123  a(n) = number of (s(0),s(1),...,s(n)) such that every s(i) is a
 *          nonnegative integer, s(0) = 1, s(n) = 2, |s(1) - s(0)| = 1, |s(i)
 *          - s(i-1)| <= 1 for i >= 2. Also a(n) = T(n,n-1), where T is the
 *          array in A026120; a(n) = U(n,n+1), where U is the array in
 *          A026148.
 * https://oeis.org/A026123
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (n+5)*a(n) +5*(-n-3)*a(n-1) +(5*n+1)*a(n-2)
 *            +(5*n+3)*a(n-3) +6*(-n+3)*a(n-4)=0 for all n ≥ 6, with a(2)=1,
 *            a(3)=4, a(4)=10, a(5)=28. This code uses an O(n) recurrence and
 *            memoises computed values
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoisation table */
static size_t cache_size = 0; /* current allocated size */

/* Ensure space for indices 0 .. n (inclusive) and initialise the base values. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;

    size_t newsize = (size_t)n + 1;
    seq_t *newcache = NULL;

#ifdef __cplusplus
    newcache = new seq_t[newsize];
    if (cache) {
        std::copy(cache, cache + cache_size, newcache);
        delete[] cache;
    }
#else
    newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newcache) abort();          /* allocation failure */
    if (!cache) memset(newcache, 0, newsize * sizeof(seq_t));
#endif

    /* Fill any newly allocated slots with zero */
    if (cache_size < newsize)
        memset(newcache + cache_size, 0, (newsize - cache_size) * sizeof(seq_t));

    cache    = newcache;
    cache_size = newsize;

    /* Initialise the known sequence values at indices 2..5 */
    if (cache_size > 2) cache[2] = (seq_t)1;   /* a(2) */
    if (cache_size > 3) cache[3] = (seq_t)4;   /* a(3) */
    if (cache_size > 4) cache[4] = (seq_t)10;  /* a(4) */
    if (cache_size > 5) cache[5] = (seq_t)28;  /* a(5) */
}

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Values below the offset are undefined; return 0. */
        return (seq_t)0;
    }

    /* Make sure the cache can hold up to index n and that the base values are set. */
    ensure_cache(n);

    if (n <= 5) {            /* already known */
        return cache[n];
    }

    /* Compute values up to n using the recurrence for n ≥ 6. */
    for (idx_t i = 6; i <= n; ++i) {
        /* Use seq_t arithmetic to avoid overflow of intermediate 64‑bit values. */
        seq_t coeff1 = (seq_t)5 * (seq_t)i + (seq_t)15; /* 5n + 15  */
        seq_t coeff2 = (seq_t)5 * (seq_t)i + (seq_t)1;  /* 5n + 1   */
        seq_t coeff3 = (seq_t)5 * (seq_t)i + (seq_t)3;  /* 5n + 3   */
        seq_t coeff4 = (seq_t)6 * (seq_t)i - (seq_t)18; /* 6n – 18  */

        seq_t term1 = coeff1 * cache[i - 1];
        seq_t term2 = coeff2 * cache[i - 2];
        seq_t term3 = coeff3 * cache[i - 3];
        seq_t term4 = coeff4 * cache[i - 4];

        /* Numerator = term1 - term2 - term3 + term4
           Compute as (term1 + term4) - (term2 + term3) to avoid negative intermediates. */
        seq_t temp1 = term1 + term4;
        seq_t temp2 = term2 + term3;

        /* The recurrence guarantees temp1 ≥ temp2 for n ≥ 6. */
        seq_t numer = temp1 - temp2;

        seq_t denom = (seq_t)i + (seq_t)5;   /* n + 5 */

        cache[i] = numer / denom;
    }

    return cache[n];
}
