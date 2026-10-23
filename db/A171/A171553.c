/*
 * A171553  a(n) = 9*10^n + 100^n + 1.
 * https://oeis.org/A171553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*10^n + 100^n + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 100^n using the provided utility */
    seq_t p10  = pow_uint(10ULL, n);
    seq_t p100 = pow_uint(100ULL, n);

    /* Compute the sequence value from the formula */
    seq_t result = mul_small(p10, 9U); /* 9 * 10^n   */
    result += p100;                   /* + 100^n    */
    result += 1U;                     /* + 1        */

    return result;
}
