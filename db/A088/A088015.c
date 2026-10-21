/*
 * A088015  Expansion of e.g.f. cosh(sqrt(2)*x) + exp(x)*(cosh(sqrt(2)*x) -
 *          1).
 * https://oeis.org/A088015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-7*a(n-3)+2*a(n-4)+2*a(n-5) with a(0)=1,
 *            a(1)=0, a(2)=4, a(3)=6, a(4)=20
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;          /* cache of computed terms   */
    static idx_t buf_cap = 0;          /* capacity of buf           */
    static idx_t last = 4;             /* highest index computed    */

    /* initialise cache with known terms */
    if (!buf) {
        buf_cap = 8;                   /* initial capacity          */
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        buf[0] = 1;
        buf[1] = 0;
        buf[2] = 4;
        buf[3] = 6;
        buf[4] = 20;
        for (idx_t i = 5; i < buf_cap; ++i)
            buf[i] = 0;
    }

    /* grow capacity if needed */
    if (n + 1 > buf_cap) {
        idx_t new_cap = buf_cap;
        while (n + 1 > new_cap) new_cap <<= 1;
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        for (idx_t i = buf_cap; i < new_cap; ++i)
            buf[i] = 0;
        buf_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i < 5) {
            /* already initialised */
            continue;
        }
        seq_t val = 3 * buf[i-1] + buf[i-2]
                    - 7 * buf[i-3] + 2 * buf[i-4] + 2 * buf[i-5];
        buf[i] = val;
    }
    last = (n > last) ? n : last;

    return buf[n];
}
