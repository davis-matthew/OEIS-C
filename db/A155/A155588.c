/*
 * A155588  a(n) = 5^n + 2^n - 1^n.
 * https://oeis.org/A155588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + 2^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   pow_seq_ts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_ts.h"

seq_t a(idx_t n)
{
    seq_t five_n = pow_seq_ts((seq_t)5, n);
    seq_t two_n  = pow_seq_ts((seq_t)2, n);
    seq_t one    = (seq_t)1;
    return five_n + two_n - one;
}
