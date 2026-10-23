/*
 * A130013  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+647)^2 = y^2.
 * https://oeis.org/A130013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1294, n>6 * Initial values: * a(1)=0,
 *            a(2)=37, a(3)=1768, a(4)=1941, a(5)=2128, a(6)=11937
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 29, u64 n <= 65, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: buf[1]..buf[last] contain a(1)..a(last) */
    static seq_t *buf = NULL;
    static size_t size = 0;          /* allocated size (buf[0] unused) */
    static size_t last = 0;          /* highest index already computed */

    if (n < 1)
        return (seq_t)0;             /* out of range */

    /* Ensure buffer is large enough for indices up to n */
    if (n + 1 > size) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newsize * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;         /* allocation failure */
        buf = tmp;
        size = newsize;
        /* Reinitialize known base values (they may have been overwritten
           by realloc under extreme conditions). Only up to the allocated
           size and up to index 6. */
        for (idx_t i = 1; i <= 6 && i < size; ++i) {
            switch (i) {
            case 1: buf[1] = (seq_t)0;   break;
            case 2: buf[2] = (seq_t)37;  break;
            case 3: buf[3] = (seq_t)1768;break;
            case 4: buf[4] = (seq_t)1941;break;
            case 5: buf[5] = (seq_t)2128;break;
            case 6: buf[6] = (seq_t)11937;break;
            }
        }
    }

    /* Compute missing values up to index n using the recurrence */
    if (last < n) {
        idx_t start = (last + 1 > 6) ? last + 1 : 7;
        for (idx_t i = start; i <= n; ++i) {
            buf[i] = 6 * buf[i - 3] - buf[i - 6] + (seq_t)1294;
        }
        last = n;
    }

    return buf[n];
}
