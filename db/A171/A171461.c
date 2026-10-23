/*
 * A171461  a(n) = 1 + 6*10^n + 100^n.
 * https://oeis.org/A171461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6 * 10^n + 100^n Recurrence: a(n) = 111*a(n-1) -
 *            1110*a(n-2) + 1000*a(n-3) for n>2 We compute directly using the
 *            pow10 helper (10^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p10   = pow10(n);    /* 10^n */
    seq_t p100  = p10 * p10;   /* (10^n)^2 = 100^n */
    seq_t res   = p100 + ((seq_t)6 * p10) + (seq_t)1;
    return res;
}
