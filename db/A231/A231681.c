/*
 * A231681  a(n) = Sum_{i=0..n} digsum_8(i)^2, where digsum_8(i) = A053829(i).
 * https://oeis.org/A231681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digsum_8(i))^2 * digsum_8(i) = sum of the
 *            digits of i when written in base 8
 * Range:     u8 n <= 13, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;                          /* running sum of squares */
    for (idx_t i = 0; i <= n; ++i) {        /* iterate all numbers 0..n */
        idx_t x = i;                        /* temporary copy of i */
        idx_t dsum = 0;                     /* digit sum in base 8 */
        while (x) {                         /* parse octal digits */
            dsum += x % 8;                   /* add current digit */
            x /= 8;                          /* shift right one octal digit */
        }
        seq_t sq = (seq_t)dsum;              /* cast to seq_t before sq */
        ans += sq * sq;                     /* accumulate square */
    }
    return ans;
}
