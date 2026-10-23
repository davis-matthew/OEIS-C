/*
 * A171410  a(n) = 1 + 5*10^n + 100^n.
 * https://oeis.org/A171410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 5*10^n + 100^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 100^n (which is 10^(2*n)) */
    seq_t pow10n   = pow10(n);
    seq_t pow100n  = pow10(2 * n);

    /* a(n) = 1 + 5*10^n + 100^n */
    seq_t result = pow100n;
    result += pow10n * (seq_t)5;
    result += (seq_t)1;

    return result;
}
