/*
 * A220989  a(n) = 12^(2n+1) - 6 * 12^n + 1: the left Aurifeuillian factor of
 *          12^(6n+3) + 1.
 * https://oeis.org/A220989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(2n+1) - 6 * 12^n + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow12n    = big_pow((seq_t)12, (seq_t)n);
    seq_t pow12_2n1 = big_pow((seq_t)12, (seq_t)(2 * n + 1));
    seq_t result    = pow12_2n1 - pow12n * (seq_t)6 + (seq_t)1;
    return result;
}
