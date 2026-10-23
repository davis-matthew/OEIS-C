/*
 * A122519  Expansion of x * (x+1) * (x^3-x^2-1) / ((x^2+1) * (x^3+x^2-1)).
 * https://oeis.org/A122519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 42, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122519: Expansion of x*(x+1)*(x^3-x^2-1) / ((x^2+1)*(x^3+x^2-1))
   Recurrence: a(n) = a(n-3) + a(n-4) + a(n-5)  for n >= 6
   Initial values: a(1)=a(2)=a(3)=a(4)=a(5)=1   (offset 1) */

#include <stddef.h>   /* for idx_t, seq_t types defined by the build */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n < 1, but return 0 as sentinel */

    if (n <= 5)
        return 1;    /* first five terms are 1 */

    /* Rolling window holding a(i-1), a(i-2), ..., a(i-5) */
    seq_t r0 = 1, /* a(i-1) */
          r1 = 1, /* a(i-2) */
          r2 = 1, /* a(i-3) */
          r3 = 1, /* a(i-4) */
          r4 = 1; /* a(i-5) */

    seq_t ai; /* current term a(i) */

    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = a(i-3) + a(i-4) + a(i-5) */
        ai = r2 + r3 + r4;

        /* shift window for next iteration:
           new r0 = a(i),  r1 = old r0, r2 = old r1, r3 = old r2, r4 = old r3 */
        r4 = r3;
        r3 = r2;
        r2 = r1;
        r1 = r0;
        r0 = ai;
    }

    return r0;   /* after the loop r0 holds a(n) */
}
