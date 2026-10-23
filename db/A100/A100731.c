/*
 * A100731  a(n) = 3^(n!).
 * https://oeis.org/A100731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^n, with a(0) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper function pow_seq is available for use in the build. */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i)
        result = pow_seq(result, i);
    return result;
}
