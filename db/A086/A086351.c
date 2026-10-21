/*
 * A086351  Row T(n,3) of square array A086350.
 * https://oeis.org/A086351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached values of the sequence   */
static idx_t cached_n = (idx_t)-1; /* largest n for which cache[n] is valid */
static idx_t cache_size = 0;       /* current allocation size */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* first time we need to allocate storage */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* allocation failure */

        cache[0] = 1;
        cache[1] = 4;
        cached_n = 1;

        /* compute values up to n */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t term = (seq_t)(((__uint128_t)6) * cache[i-1] -
                                 (__uint128_t)7 * cache[i-2]);
            cache[i] = term;
        }
        cached_n = n;
        return cache[n];
    }

    /* if the requested n is already cached, return it */
    if (n <= cached_n)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute new terms */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* i==1 is already initialized, skip */
        seq_t term = (seq_t)(((__uint128_t)6) * cache[i-1] -
                             (__uint128_t)7 * cache[i-2]);
        cache[i] = term;
    }

    cached_n = n;
    return cache[n];
}
