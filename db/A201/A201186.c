/*
 * A201186  Incorrect list of perfect numbers from Robert Recorde's Whetstone
 *          of Witte.
 * https://oeis.org/A201186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = * 6 if n = 1 * 2^(2*n-2) * (2^(2*n-1)-1) otherwise *
 *            where 2^(2*n-1)-1 is the (2*n-1)-th Mersenne number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Special case n = 1 */
    if (n == 1) {
        return (seq_t)6;
    }
    /* For n >= 2, compute p = 2*n - 1 */
    seq_t p = ((seq_t)n) * 2 - 1;
    seq_t two = (seq_t)2;
    /* Compute 2^(p-1) and 2^p */
    seq_t pow_p_minus_1 = big_pow(two, p - 1);
    seq_t pow_p         = big_pow(two, p);
    /* a(n) = 2^(p-1) * (2^p - 1) */
    return pow_p_minus_1 * (pow_p - (seq_t)1);
}
