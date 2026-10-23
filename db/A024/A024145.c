/*
 * A024145  a(n) = 12^n - n^5.
 * https://oeis.org/A024145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n - n^5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 12^n is big_pow(12, n) and n^5 is pow_int(n, 5) */
    return big_pow((seq_t)12, (seq_t)n) - pow_int((seq_t)n, 5);
}
