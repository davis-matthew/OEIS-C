/*
 * A166481  a(n) = 3*a(n-2) for n > 2; a(1) = 1; a(2) = 7.
 * https://oeis.org/A166481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) for n > 2; a(1) = 1; a(2) = 7;
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 159,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1, return 0 safely */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    if (n & 1U) { /* odd n -> a(2k+1) = 3^k */
        idx_t k = (n - 1) >> 1;
        return pow3(k);
    } else { /* even n -> a(2k) = 7 * 3^(k-1) */
        idx_t k = n >> 1;          /* k >= 1 because n >= 2 */
        seq_t r = pow3(k - 1);
        return (seq_t)7 * r;
    }
}
