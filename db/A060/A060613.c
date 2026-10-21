/*
 * A060613  Number of n X n {-1,0,1} matrices with no zero rows.
 * https://oeis.org/A060613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow_seq, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pow3 = pow_uint(3, n);
    seq_t base = pow3 - 1;
    seq_t result = pow_seq(base, (int)n);
    return result;
}
