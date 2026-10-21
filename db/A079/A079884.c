/*
 * A079884  Number of comparisons required to create all permutations of n
 *          distinct elements using the "streamlined" version of Algorithm L
 *          (lexicographic permutation generation) from Knuth's The Art of
 *          Computer Programming, Vol. 4, chapter 7.2.1.2.
 * https://oeis.org/A079884
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + n*(n+1)/2 (n >= 4, a(3) = 11)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment. */
extern seq_t a(idx_t n)
{
    /* Static cache of already computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_upto = 0;    /* largest index that has been computed */

    /* Context: the function is called only for n >= 3.  We initialise
     * the cache on the first call.  cache[3] holds the base term. */
    if (cache == NULL)
    {
        cache_size = 4;                 /* indices 0 .. 3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0;    /* memory‑allocation failure */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
        cache[3] = (seq_t)11;           /* a(3) */
        computed_upto = 3;
    }

    /* Grow the cache array if the requested index is beyond the current
     * allocated size.  We grow it exactly to n+1 to store all terms up
     * to n.  Newly allocated slots are zeroed. */
    if (n + 1 > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return 0;     /* memory‑allocation failure */
        cache = tmp;
        cache_size = n + 1;
        for (idx_t i = computed_upto + 1; i <= n; ++i) cache[i] = 0;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = computed_upto + 1; i <= n; ++i)
    {
        seq_t part1 = (seq_t)i * cache[i - 1];
        seq_t part2 = (seq_t)i * ((seq_t)i + 1) / 2;
        cache[i] = part1 + part2;
    }
    computed_upto = n;

    return cache[n];
}
