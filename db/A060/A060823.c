/*
 * A060823  4-wave sequence beginning with 2's with middles dropped.
 * https://oeis.org/A060823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4) with a(2)=2,
 *            a(3)=2, a(4)=8, a(5)=20. Index offset is 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>          /* for placement new when seq_t is a class type */
#endif

static seq_t *cache = NULL;        /* dynamic array holding computed terms       */
static idx_t cache_size  = 0;      /* number of allocated elements (including 0) */
static idx_t computed_upto = 1;    /* highest index that has been computed      */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are undefined */

    /* Grow the cache if necessary. */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;               /* indices 0 .. n */
        void *raw = realloc(cache, new_size * sizeof(seq_t));
        if (!raw) return 0;                   /* allocation failed */

        seq_t *tmp = (seq_t *)raw;
#ifdef __cplusplus
        /* Construct any newly allocated elements. */
        for (idx_t i = cache_size; i < new_size; ++i)
            new (&tmp[i]) seq_t();
#endif
        cache     = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i == 2)
            cache[i] = 2;
        else if (i == 3)
            cache[i] = 2;
        else if (i == 4)
            cache[i] = 8;
        else if (i == 5)
            cache[i] = 20;
        else
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)3
                       - cache[i-3] - cache[i-4];
    }
    if (n > computed_upto) computed_upto = n;

    return cache[n];
}
