/*
 * A092384  Sum of digits of n if n even, else sum of digits of 2n.
 * https://oeis.org/A092384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of the decimal digits of (n if n is even, otherwise
 *            2*n)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the digit sum of a value that fits in unsigned __int128.
   The result always fits in seq_t (maximum sum 9*20 = 180). */
static seq_t digit_sum_uint128(unsigned __int128 x)
{
    seq_t sum = 0;
    while (x) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    unsigned __int128 val;
    if ((n & 1U) == 0)          /* n even: use n itself */
        val = (unsigned __int128)(seq_t)n;
    else                        /* n odd: use 2*n */
        val = (unsigned __int128)(seq_t)n * 2ULL;
    return digit_sum_uint128(val);
}
