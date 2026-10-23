/*
 * A036088  Centered cube numbers: (n+1)^10 + n^10.
 * https://oeis.org/A036088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)^10 + n^10
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 78, u128 n <= 6653,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_seq(seq_t base)
{
    seq_t result = 1;
    for (int i = 0; i < 10; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* convert n to seq_t first; add 1 after casting to avoid idx_t overflow */
    seq_t sn     = (seq_t)n;
    seq_t snplus = sn + 1;
    seq_t n10    = pow10_seq(sn);
    seq_t n1_10  = pow10_seq(snplus);
    return n10 + n1_10;
}
