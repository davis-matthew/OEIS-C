/*
 * A134183  A Hankel transform of a Catalan product.
 * https://oeis.org/A134183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 2^(n^2 - 1), with a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t exp = n * n - 1;          /* exponent of 2 */
    seq_t res = pow2(exp);          /* 2^exp */
    return res * (seq_t)n;          /* multiply by n */
}
