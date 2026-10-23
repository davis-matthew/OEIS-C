/*
 * A014170  Inverse of 161st cyclotomic polynomial.
 * https://oeis.org/A014170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 161 ∈ [0,6] * a(n) = -1 if n mod 161 ∈ [23,29]
 *            * a(n) = 0 otherwise * The sequence is the coefficient sequence
 *            of 1/Φ_161(x), * where Φ_161 is the 161‑st cyclotomic polynomial
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* The period is 161, so we only need n modulo 161. */
    idx_t r = n % (idx_t)161;

    /* Map the residue r to the corresponding term. */
    if (r <= 6)          return 1;
    if (r >= 23 && r <= 29) return -1;
    return 0;
}
