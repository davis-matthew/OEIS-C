/*
 * A022540  Nexus numbers (n+1)^24 - n^24.
 * https://oeis.org/A022540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^24 - n^24
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t np1 = n_val + (seq_t)1;
    seq_t p_np1 = big_pow(np1, (seq_t)24);
    seq_t p_n   = big_pow(n_val, (seq_t)24);
    return p_np1 - p_n;
}
