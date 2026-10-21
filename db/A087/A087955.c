/*
 * A087955  a(n) is the square of the n-th partial sum minus the n-th partial
 *          sum of the squares, divided by a(n-1), for all n>=1, starting with
 *          a(0)=1, a(1)=2.
 * https://oeis.org/A087955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) for n > 3 with a(0) = 1, a(1)
 *            = 2, a(2) = 2, a(3) = 8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for abort */

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* simple base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;

    /* initialise cache once */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)8;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i-1] + (cache[i-2] * (seq_t)3) - cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
