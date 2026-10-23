/*
 * A129905  Expansion of g.f.: (1-x)*(1+2*x)/((1+x)*(1-3*x+x^2)).
 * https://oeis.org/A129905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) Initial values: a(0)=1,
 *            a(1)=3, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached terms */
    static size_t  cap   = 0;       /* allocated size */
    size_t need_cap = (size_t)n + 1;

    if (need_cap > cap) {
        /* grow cache to hold terms up to n */
        seq_t *new_ptr = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
        if (!new_ptr)                 /* allocation failure – unlikely */
            return (seq_t)0;
        /* initialize newly allocated entries */
        for (size_t i = cap; i < need_cap; ++i) {
            if (i == 0)               new_ptr[i] = (seq_t)1;
            else if (i == 1)          new_ptr[i] = (seq_t)3;
            else if (i == 2)          new_ptr[i] = (seq_t)6;
            else                      new_ptr[i] = (seq_t)2 * new_ptr[i-1]
                                      + (seq_t)2 * new_ptr[i-2]
                                      - new_ptr[i-3];
        }
        cache = new_ptr;
        cap   = need_cap;
    }
    return cache[(size_t)n];
}
