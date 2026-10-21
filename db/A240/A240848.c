/*
 * A240848  Sum of n, digitsum(n) and number of digits of n.
 * https://oeis.org/A240848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + digit_sum(n) + digit_len(n)
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t ds = digit_sum(sn);
    int    dl = digit_len(sn);
    return sn + ds + (seq_t)dl;
}
