/*
 * A035277  One eighth of deca-factorial numbers.
 * https://oeis.org/A035277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = a(n‑1) * (10*n – 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A035277.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] holds a(1)  */
    static idx_t size = 0;        /* current number of cached terms */

    if (n == 0) return 0;    /* safety, although n is assumed ≥ 1 */

    if (size == 0) {          /* initialise the cache */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;           /* a(1) = 1 */
        size = 1;
    }

    if (size < n) {           /* extend cache up to n */
        size_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed, return 0 as error sentinel */
        cache = tmp;

        /* compute successive terms */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t factor = ((seq_t)10 * (seq_t)i) - (seq_t)2;
            cache[i-1] = cache[i-2] * factor;
        }
        size = n;
    }

    return cache[n-1];        /* a(n) */
}
