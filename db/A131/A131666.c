/*
 * A131666  First differences of (A113405 prefixed with a 0).
 * https://oeis.org/A131666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(n+1)=2*a(n)+b(n) with b(n) repeating
 *            0,1,-1,0,-1,1 (hexaperiodic A092220)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t size = 0, cap = 0;
    /* distillation of the hexaperiodic sequence */
    static const int pattern[6] = {0, 1, -1, 0, -1, 1};

    /* trivial base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;

    /* initialise cache if necessary */
    if (size == 0) {
        cap = 1;
        size = 1;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = 0;                     /* a(0) */
    }

    /* extend cache up to n */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            /* compute a(i) using a(i-1) */
            seq_t prev = cache[i - 1];
            seq_t val  = prev << 1;            /* 2 * a(i-1) */

            int b = pattern[(i - 1) % 6];
            if (b > 0)
                val += (seq_t)b;
            else if (b < 0)
                val -= (seq_t)(-b);

            /* grow cache if needed */
            if (size == cap) {
                cap = cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size++] = val;                 /* store a(i) */
        }
    }

    return cache[n];
}
