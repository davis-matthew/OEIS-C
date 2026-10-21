/*
 * A060226  a(n) = n^n - n*(n-1)^(n-1).
 * https://oeis.org/A060226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - n*(n-1)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) /* by convention 0^0 = 1 and a(0) = 1 */
        return 1;

    /* n^n */
    seq_t pow_n = pow_uint((uint64_t)n, n);

    /* (n-1)^(n-1) */
    seq_t pow_n_minus = pow_uint((uint64_t)(n - 1), n - 1);

    /* n * (n-1)^(n-1) */
    seq_t prod = ((seq_t)n) * pow_n_minus;

    /* a(n) = n^n - n*(n-1)^(n-1) */
    return pow_n - prod;
}
