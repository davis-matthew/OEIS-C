/*
 * A090633  Start with the sequence [1, 1/2, 1/3, ..., 1/n]; form new sequence
 *          of n-1 terms by taking averages of successive terms; repeat until
 *          reach a single number F(n); a(n) = numerator of F(n).
 * https://oeis.org/A090633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t p = pow2(n);

    /* Compute numerator 2^n - 1 */
    seq_t num = p - (seq_t)1;

    /* Compute gcd(2^n - 1, n) */
    seq_t d = gcd(num, (seq_t)n);

    /* Simplify: (2^n - 1) / gcd(2^n - 1, n) */
    return num / d;
}
