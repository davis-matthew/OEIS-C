/*
 * A028103  Expansion of 1/((1-3x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (35*12^(n+3)-9*10^(n+4)+63*8^(n+3)-8*3^(n+3))/2520
 *            Sequence A028103: coefficients of 1/((1-3x)(1-8x)(1-10x)(1-12x))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values from the sequence */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)33;
    if (n == 2) return (seq_t)703;
    if (n == 3) return (seq_t)12309;

    /* Linear recurrence: a(n) = 33*a(n-1) - 386*a(n-2)
                               + 1848*a(n-3) - 2880*a(n-4)   */
    seq_t a0 = (seq_t)1;       /* a(n-4)   */
    seq_t a1 = (seq_t)33;      /* a(n-3)   */
    seq_t a2 = (seq_t)703;     /* a(n-2)   */
    seq_t a3 = (seq_t)12309;   /* a(n-1)   */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)33 * a3
            - (seq_t)386 * a2
            + (seq_t)1848 * a1
            - (seq_t)2880 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
