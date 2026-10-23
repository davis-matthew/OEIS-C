/*
 * A018070  Expansion of 1/((1-3*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A018070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 145*a(n-2) + 264*a(n-3) for n>2; a(0) = 1,
 *            a(1) = 22, a(2) = 339
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Handle the three initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)339;

    /* Iteratively compute a(n) using only the last three values. */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)22;     /* a(1) */
    seq_t a2 = (seq_t)339;    /* a(2) */
    seq_t a3;                 /* placeholder for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 22 * a2 - 145 * a1 + 264 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
