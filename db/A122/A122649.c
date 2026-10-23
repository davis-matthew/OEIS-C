/*
 * A122649  Difference between the double factorial of the n-th nonnegative
 *          odd number and the double factorial of the n-th nonnegative even
 *          number.
 * https://oeis.org/A122649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n-5)*a(n-1) - (2*n-4)*(2*n-3)*a(n-2), a(1)=0, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A122649, difference of (2n-1)!! and (2n-2)!! */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cached values for earlier indices – memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* largest n already computed */
    static size_t cache_cap = 0;   /* allocated capacity (indexable up to cache_cap-1) */

    /* Handle the trivial small indices directly */
    if (n == 0)
        return 0;
    if (n == 1)
        return 0;
    if (n == 2)
        return 1;

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate indices 0..2 (0 unused) */
        cache_cap = 3;
        cache_len = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;              /* allocation failure – unlikely in test harness */
        memset(cache, 0, cache_cap * sizeof(seq_t));
        cache[1] = 0;
        cache[2] = 1;
    }

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;              /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
        /* initialise new memory to zero for safety */
        memset(cache + cache_len + 1, 0, (new_cap - cache_len - 1) * sizeof(seq_t));
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];

        seq_t term1 = ((seq_t)4 * (seq_t)i - (seq_t)5) * prev1;
        seq_t factorA = (seq_t)2 * (seq_t)i - (seq_t)4;
        seq_t factorB = (seq_t)2 * (seq_t)i - (seq_t)3;
        seq_t term2 = factorA * factorB * prev2;

        cache[i] = term1 - term2;
    }
    cache_len = (size_t)n;

    return cache[n];
}
