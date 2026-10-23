/*
 * A171498  a(n) = 4*3^n-1.
 * https://oeis.org/A171498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^n-1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t res  = seq_mul(pow3, (seq_t)4) - (seq_t)1;
    return res;
}
