/*
 * A050250  Number of nonzero palindromes less than 10^n.
 * https://oeis.org/A050250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(2k) = 2*10^k - 2, a(2k+1) = 11*10^k - 2, k = ⌊n/2⌋
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term, values for n < 1 are undefined; return 0. */
    if (n == 0) return 0;

    /* k = floor(n/2) */
    idx_t k = n / 2;

    /* Compute 10^k once, using the utility power-of-ten routine. */
    seq_t p = pow10(k);

    if (n & 1) {
        /* n is odd: a(2k+1) = 11*10^k - 2 */
        return p * (seq_t)11 - (seq_t)2;
    } else {
        /* n is even: a(2k) = 2*10^k - 2 */
        return p * (seq_t)2 - (seq_t)2;
    }
}
