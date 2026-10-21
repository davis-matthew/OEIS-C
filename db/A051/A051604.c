/*
 * A051604  a(n) = (3*n+4)!!!/4!!!.
 * https://oeis.org/A051604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} (3k + 4), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A051604:
 * a(n) = (3*n+4)!!! / 4!!!  = product_{k=0..n} (3k+4) / 4
 *        = product_{k=1..n} (3k+4)  (since the 4 cancels the first factor)
 *   a(0) = 1
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        result = result * (seq_t)(3 * k + 4);
    }
    return result;
}
