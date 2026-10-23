/*
 * A194566  Centered cube numbers: (n+1)^100+n^100.
 * https://oeis.org/A194566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^100 + n^100
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 0, u128 n <= 1,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t type for accurate arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t pow_n  = big_pow(sn,          (seq_t)100);  // n^100
    seq_t sn1  = sn + 1;
    seq_t pow_n1 = big_pow(sn1,          (seq_t)100);  // (n+1)^100
    return pow_n + pow_n1;
}
