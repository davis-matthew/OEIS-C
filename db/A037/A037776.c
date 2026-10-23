/*
 * A037776  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3; for n>1, a(n)=8*a(n-1)+d_n, where d_n is the n-th
 *            element of the repeating pattern 3,1,0,2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined offset, return 0 */

    static seq_t *cache = NULL;
    static size_t cap = 0;

    if ((size_t)n > cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();              /* allocation failed */
        cache = tmp;

        if (cap == 0) {
            cache[0] = (seq_t)3;  /* a(1) = 3 */
            cap = 1;
        }

        /* digits repeated 3,1,0,2 */
        static const unsigned char digits[4] = {3, 1, 0, 2};

        for (size_t i = cap; i < newcap; ++i) {
            seq_t prev = cache[i - 1];
            unsigned char d = digits[i % 4];
            cache[i] = prev * (seq_t)8 + (seq_t)d;
        }
        cap = newcap;
    }

    return cache[n - 1];
}
