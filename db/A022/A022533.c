/*
 * A022533  Nexus numbers (n+1)^17 - n^17.
 * https://oeis.org/A022533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^17 - n^17
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 12, u128 n <= 213,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t next_val = (seq_t)(n + 1);
    seq_t pow_next = big_pow(next_val, (seq_t)17);
    seq_t pow_cur  = big_pow(n_val, (seq_t)17);
    return pow_next - pow_cur;
}
