/*
 * A180142  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 + x
 *          - x^2)/(1 - 3*x - 3*x^2).
 * https://oeis.org/A180142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(1) = 4 a(2) = 14 a(n) = 3*a(n-1) + 3*a(n-2) for n >=
 *            3 (the recurrence given for n ≥ 2 in the OEIS entry, with
 *            a(2)=14 supplied as a base value)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* External types provided by the build */
seq_t a(idx_t n)
{
    /* -------------  static cache  ------------- */
    static seq_t *cache   = NULL;   /* array of previously computed terms */
    static idx_t  cache_sz = 0;     /* number of elements currently stored */

    /* -------------  initialise cache  ------------- */
    if (cache == NULL) {
        /* allocate room for the first three terms */
        size_t init_sz = 3;
        cache = (seq_t *)malloc(init_sz * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        cache_sz = init_sz;
    }

    /* -------------  request already computed  ------------- */
    if (n < cache_sz) {
        return cache[n];
    }

    /* -------------  grow cache up to n  ------------- */
    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* -------------  compute missing terms by recurrence  ------------- */
    /* index i starts at the current cache_sz (first missing term) */
    for (idx_t i = cache_sz; i <= n; ++i) {
        /* recurrence is valid for i >= 3; we ensure i-1 and i-2 exist */
        cache[i] = cache[i-1] * (seq_t)3 + cache[i-2] * (seq_t)3;
    }

    cache_sz = new_sz;
    return cache[n];
}
