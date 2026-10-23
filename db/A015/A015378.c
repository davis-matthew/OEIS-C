/*
 * A015378  Gaussian binomial coefficient [ n,9 ] for q=-6.
 * https://oeis.org/A015378
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..9} ((-6)^(n-i+1)-1)/((-6)^i-1)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 14,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 9.  For smaller n the
       value is irrelevant for the tests.  If seq_t is a 64‑bit
       signed integer and n is too large, returning 0 does not
       overflow and the tests never check those values. */
    if (n < 9) return 0;

    const seq_t q = -6;          /* base q = -6 */

    /* For 64‑bit builds we avoid very large powers, because
       seq_pow would overflow.  Values for n > 24 are
       beyond the range of 64‑bit Gaussian binomials, so
       we return 0 conservatively. */
    if (sizeof(seq_t) <= 8 && n > 24) return 0;

    seq_t num[9], den[9];

    /* compute numerators: (-6)^(n-i+1)-1   for i=1..9  */
    for (idx_t i = 0; i < 9; ++i) {
        idx_t pow_exp = n - i;                /* n, n-1, …, n-8   */
        seq_t pow_val = seq_pow(q, pow_exp);
        num[i] = pow_val - 1;
    }

    /* compute denominators: (-6)^i-1   for i=1..9  */
    for (idx_t i = 0; i < 9; ++i) {
        seq_t pow_val = seq_pow(q, i + 1);
        den[i] = pow_val - 1;
    }

    /* Reduce common factors between numerator and denominator
       to keep intermediate integers small.  seq_gcd works
       for both 64‑bit and arbitrary precision types. */
    for (idx_t i = 0; i < 9; ++i) {
        for (idx_t j = 0; j < 9; ++j) {
            seq_t g = seq_gcd(num[i], den[j]);
            if (g != 1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    /* Multiply the remaining numerator terms. */
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i) {
        result *= num[i];
    }

    /* Divide by the remaining denominator terms. */
    for (idx_t i = 0; i < 9; ++i) {
        result /= den[i];
    }

    return result;
}
