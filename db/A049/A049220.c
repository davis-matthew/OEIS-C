/*
 * A049220  Number of horizontally convex n-ominoes in which the top row has
 *          at least 2 squares and the rightmost square in the top row is
 *          above the leftmost square in the second row.
 * https://oeis.org/A049220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=9
 *            a(n)=5*a(n-1)-7*a(n-2)+4*a(n-3) for n>=6
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Cached values of the sequence.
   Indices 1..n correspond to a(1)..a(n).  */
static seq_t *cache = NULL;      /* pointer to array (0..size-1) */
static idx_t  cache_size   = 0;  /* allocated size (>=0)           */
static idx_t  computed_max = 0;  /* highest n for which cache[n] is set */

static void
ensure_cache(idx_t n)
{
    /* Keep the array at least requested width (n+1).  */
    if (cache_size > n)
        return;

    idx_t new_size = (cache_size == 0) ? (n + 1) : cache_size;
    while (new_size <= n)
        new_size <<= 1;          /* grow by factor 2 */

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_size];
#else
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return;                 /* out of memory – caller will fallback to 0 */
#endif

    /* Copy old data.  The other elements are uninitialized
       and will be filled when needed. */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t
a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n < 1 we return 0. */
    if (n < 1)
        return 0;

    /* Base values for n = 1..5 */
    static const seq_t base[6] = {0, 0, 0, 1, 3, 9};

    if (n <= 5)
        return base[n];

    /* Make the cache large enough. */
    ensure_cache(n);

    /* Initialize base entries once. */
    if (computed_max < 5) {
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = base[i];
        computed_max = 5;
    }

    /* Compute missing values up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term = 5 * cache[i - 1];
        term -= 7 * cache[i - 2];
        term += 4 * cache[i - 3];
        cache[i] = term;
    }

    computed_max = n;
    return cache[n];
}
