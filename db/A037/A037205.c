/*
 * A037205  a(n) = (n+1)^n - 1.
 * https://oeis.org/A037205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n + 1);
    seq_t exp  = (seq_t)n;
    seq_t res  = big_pow(base, exp);
    return res - 1;
}
