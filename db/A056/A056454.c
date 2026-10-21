/*
 * A056454  Number of palindromes of length n using exactly three different
 *          symbols.
 * https://oeis.org/A056454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^ceil(n/2) - 3*2^ceil(n/2) + 3 which is equivalent to
 *            a(n) = 3 * Stirling2(ceil(n/2), 3)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) = (n+1)/2 using integer division */
    idx_t m = (n + 1) / 2;

    /* Compute 3^m and 2^m using the provided pow_seq helper */
    seq_t pow3 = pow_seq((seq_t)3, m);
    seq_t pow2 = pow_seq((seq_t)2, m);

    /* a(n) = 3^m - 3*2^m + 3 */
    seq_t result = pow3;
    result -= (seq_t)3 * pow2;
    result += (seq_t)3;

    return result;
}
