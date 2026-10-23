/*
 * A022229  Gaussian binomial coefficients [ n,11 ] for q = 6.
 * https://oeis.org/A022229
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n,11]₆ = Product_{i=1..11} (6^{n-i+1}-1)/(6^i-1)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Gaussian binomial coefficient [n,11] for q = 6.
   The function uses the recurrence a(n) = a(n−1) * (6^n−1)/(6^{n-11}−1)
   with base a(11) = 1.  For n < 11 the value is undefined and 0 is
   returned.  The implementation is safe for arbitrary‑precision
   seq_t types and runs in O(n) time. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 11)
        return 0;                /* undefined for offsets below 11 */

    if (n == 11)
        return 1;                /* base case */

    const seq_t six = (seq_t)6u;
    seq_t result = (seq_t)1;

    /* Use the recurrence a(n) = a(n-1) * (6^n-1) / (6^{n-11}-1) */
    for (idx_t k = 12; k <= n; ++k) {
        seq_t num = big_pow(six, (seq_t)k) - (seq_t)1;
        seq_t den = big_pow(six, (seq_t)(k - 11)) - (seq_t)1;
        result = result * num;   /* multiply by numerator */
        result = result / den;   /* exact division by denominator */
    }

    return result;
}
