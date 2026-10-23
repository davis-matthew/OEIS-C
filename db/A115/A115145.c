/*
 * A115145  Sixth convolution of A115140.
 * https://oeis.org/A115145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 13, i32 n <= 21, i64 n <= 38, i128 n <= 70,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115145
 * Binomial formula: a(n) = -6 * binomial(2*n-7, n-6) / n   (n ≥ 6)
 * Alternative recurrence: n*(n-6)*a(n) - 2*(2*n-7)*(n-4)*a(n-1) = 0,
 *  with a(0)=1, a(1)=-6, a(2)=9, a(3)=-2, a(4)=a(5)=0, a(6)=-1.
 * This implementation uses the recurrence and reduces
 * intermediate numbers by GCD to avoid overflow when
 * seq_t is 64‑bit.  The arbitrary‑precision build works with the
 * same code because all operations are on seq_t values.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 6 */
    if (n <= 5) {
        static const seq_t init[6] = { 1, -6, 9, -2, 0, 0 };
        return init[n];
    }
    if (n == 6)
        return (seq_t)-1;

    seq_t prev = (seq_t)-1;          /* a(6) */
    for (idx_t i = 7; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) * num / den
           num = 2*(2*i-7)*(i-4)
           den = i*(i-6)                                */
        seq_t num = (seq_t)2 * (seq_t)(2*i - 7) * (seq_t)(i - 4);
        seq_t den = (seq_t)i * (seq_t)(i - 6);

        /* reduce with prev */
        seq_t g1 = seq_gcd(prev >= 0 ? prev : -prev, den);
        prev /= g1;
        den /= g1;

        /* reduce num with den */
        seq_t g2 = seq_gcd(num, den);
        num /= g2;
        den /= g2;   /* should become 1 */

        /* final multiplication (no division remaining) */
        prev = prev * num;
    }
    return prev;
}
