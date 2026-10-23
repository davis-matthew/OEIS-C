/*
 * A171375  a(n) = 1+3*10^n+100^n.
 * https://oeis.org/A171375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1+3*10^n+100^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow10n  = pow_uint(10, n);
    seq_t pow100n = pow_uint(100, n);
    seq_t res = seq_mul(pow10n, (seq_t)3);
    res = res + pow100n;
    res = res + 1;
    return res;
}
