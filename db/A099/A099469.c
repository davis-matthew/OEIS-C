/*
 * A099469  Least common multiple of n and its digit sum.
 * https://oeis.org/A099469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = lcm(n, digit_sum(n))
 * Range:     u8 n <= 28, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_sum, gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/gcd_seq_t.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;            /* convert index to seq_t */
    seq_t sum = digit_sum(num);      /* digit sum of n */
    seq_t g   = gcd_seq_t(num, sum); /* gcd of n and its digit sum */
    return num / g * sum;            /* lcm(n, sum) */
}
