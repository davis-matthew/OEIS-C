/*
 * A121625  Real part of (n + n*i)^n.
 * https://oeis.org/A121625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^n * Re((1+i)^n) * where Re((1+i)^n) = Σ_{j=0..⌊n/2⌋}
 *            (-1)^j * C(n,2j)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 8, i64 n <= 14, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;  /* (0+0i)^0 = 1 by convention */

    /* Compute Re((1+i)^n) by a simple recurrence on real/imag parts. */
    seq_t real = 1;
    seq_t imag = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t new_real = real - imag;
        seq_t new_imag = real + imag;
        real = new_real;
        imag = new_imag;
    }

    /* Compute n^n iteratively.  n is small enough that a straight
     * loop of n multiplications is O(n) and fast for n ≈ 10^4. */
    seq_t power = 1;
    for (idx_t k = 0; k < n; ++k) {
        power *= (seq_t)n;
    }

    return power * real;
}
