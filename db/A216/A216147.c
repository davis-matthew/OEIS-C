/*
 * A216147  a(n) = 2*n^n + 1.
 * https://oeis.org/A216147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t pow  = big_pow(base, base);   /* compute n^n */
    return pow * 2U + 1;                /* 2 * n^n + 1 */
}
