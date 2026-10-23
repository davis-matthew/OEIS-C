/*
 * A115052  Expansion of 1/(3*x^2 - 3*x + 1)^2.
 * https://oeis.org/A115052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 18*a(n-3) - 9*a(n-4) with a(0)=1,
 *            a(1)=6, a(2)=21, a(3)=54
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 32, i64 n <= 70, i128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generated from the expansion of 1/(3*x^2 - 3*x + 1)^2 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)54;

    seq_t a0 = (seq_t)1;  /* a(n-4)   */
    seq_t a1 = (seq_t)6;  /* a(n-3)   */
    seq_t a2 = (seq_t)21; /* a(n-2)   */
    seq_t a3 = (seq_t)54; /* a(n-1)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 6*a3 - 15*a2 + 18*a1 - 9*a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
