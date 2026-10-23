/*
 * A036098  Centered cube numbers: a(n) = (n+1)^20 + n^20.
 * https://oeis.org/A036098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^20 + n^20
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 8, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq   = (seq_t)n;
    seq_t n1_seq  = n_seq + 1;
    seq_t pow_n   = big_pow(n_seq, 20);
    seq_t pow_n1  = big_pow(n1_seq, 20);
    return pow_n + pow_n1;
}
