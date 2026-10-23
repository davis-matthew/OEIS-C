/*
 * A100638  Successive powers of the matrix A=[1,2;3,4] written by rows in
 *          groups of 4.
 * https://oeis.org/A100638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-4) + 2*a(n-8) for n>8, with the base values a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4, a(5)=7, a(6)=10, a(7)=15, a(8)=22
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 106,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, undefined for 0 */

    static seq_t *cache = NULL;           /* cache of computed values */
    static size_t curSize = 0;            /* current allocated size */

    size_t nSize = (size_t)n + 1;         /* we need indices 0..n */

    /* Ensure we have space and base values */
    if (curSize < nSize) {
        size_t newSize = nSize;
        /* We must allocate at least up to index 8 for the base case */
        if (newSize < 9) newSize = 9;

        /* Reallocate cache */
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) {                       /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;

        /* If this is the first allocation or we just grew to the base size,
           initialize the first 8 terms (index 1..8). */
        if (curSize == 0 || curSize < 9) {
            cache[0] = 0;                 /* unused, for 0-index */
            cache[1] = 1;
            cache[2] = 2;
            cache[3] = 3;
            cache[4] = 4;
            cache[5] = 7;
            cache[6] = 10;
            cache[7] = 15;
            cache[8] = 22;
        }

        /* Compute new terms using the recurrence */
        size_t start = curSize;
        if (start < 9) start = 9;          /* first computed index is 9 */
        for (size_t i = start; i < newSize; ++i) {
            cache[i] = (seq_t)5 * cache[i - 4] + (seq_t)2 * cache[i - 8];
        }
        curSize = newSize;
    }

    return cache[(size_t)n];
}
