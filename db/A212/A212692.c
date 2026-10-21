/*
 * A212692  Number of (w,x,y,z) with all terms in {1,...,n} and w<|x-y|+|y-z|.
 * https://oeis.org/A212692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-2*a(n-2)-2*a(n-3)+3*a(n-4)-a(n-5). with initial
 *            values a(0)=0, a(1)=0, a(2)=6, a(3)=22, a(4)=54
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 6;
    if (n == 3) return 22;
    if (n == 4) return 54;

    seq_t a0 = 0;   /* a(n-5) */
    seq_t a1 = 0;   /* a(n-4) */
    seq_t a2 = 6;   /* a(n-3) */
    seq_t a3 = 22;  /* a(n-2) */
    seq_t a4 = 54;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i) {
        ai = 3 * a4 - 2 * a3 - 2 * a2 + 3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
