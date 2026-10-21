/*
 * A213248  Number of nonzero elements in GF(2^n) that are 13th powers.
 * https://oeis.org/A213248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) / gcd(2^n-1, 13) Since 13 divides 2^n-1 iff n
 *            is a multiple of 12, this becomes: a(n) = (2^n - 1) / 13 when
 *            n%12==0, otherwise a(n) = 2^n-1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute 2^n - 1 */
    seq_t val = pow2(n);
    val -= (seq_t)1;

    /* If n is a multiple of 12, divide by 13. */
    if ((n % 12) == 0) {
        val /= (seq_t)13;
    }

    return val;
}
