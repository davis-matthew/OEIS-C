/*
 * A099098  Quadrisection of a Padovan sequence.
 * https://oeis.org/A099098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=2*a(n-1)+3*a(n-2)+a(n-3), a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static bool  init  = false;
    static seq_t *cache = NULL; /* array of computed values */
    static idx_t  size  = 0;    /* current length of cache       */

    if (!init) {
        /* initialise cache with first three terms */
        size  = 3;
        cache = (seq_t *)calloc(size, sizeof(seq_t));
        if (!cache) return 0;                       /* out of memory – should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        init = true;
    }

    /* extend cache up to n */
    while (size <= n) {
        /* compute next term using seq_t arithmetic */
        seq_t next = cache[size - 1] * (seq_t)2
                   + cache[size - 2] * (seq_t)3
                   + cache[size - 3];
        seq_t *p = (seq_t *)realloc(cache, (size + 1) * sizeof(seq_t));
        if (!p) return 0;                      /* out of memory */
        cache = p;
        cache[size++] = next;
    }

    return cache[n];
}
