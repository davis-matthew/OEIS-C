/*
 * A240506  Number of length-n gap-free words on {1,2,3}.
 * https://oeis.org/A240506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n + 2 for n>0, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return 1;
    }

    seq_t pow3 = pow_uint((uint64_t)3, n);
    seq_t pow2 = pow_uint((uint64_t)2, n);

    return pow3 - pow2 + (seq_t)2;
}
