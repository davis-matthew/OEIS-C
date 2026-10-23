/*
 * A159217  1/8 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 17.
 * https://oeis.org/A159217
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 3 a(n) = a(n-1) + 2^(n-1) + 2^floor((n-3)/2) for n ≥ 3
 *            which is equivalent to a(n) = 2^n + c(n) with c(2) = –1 and c(n)
 *            = c(n‑1) + 2^floor((n-3)/2) for n ≥ 3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are not used */
        return (seq_t)0;

    /* a(2) = 3 */
    seq_t val = (seq_t)3;
    if (n == 2)
        return val;

    for (idx_t k = 3; k <= n; ++k) {
        seq_t inc = pow2(k - 1) + pow2((k - 3) / 2);
        val += inc;
    }

    return val;
}
