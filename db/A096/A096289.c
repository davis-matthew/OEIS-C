/*
 * A096289  Sum of digits of n in bases 2 and 5.
 * https://oeis.org/A096289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = popcount(n) + digit_sum_base5(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of digits of n in bases 2 and 5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* popcount of n */
    int pc = 0;
    idx_t m = n;
    while (m) {
        ++pc;
        m &= m - 1;               /* remove lowest set bit */
    }

    /* digit sum of n in base 5 */
    seq_t sum = (seq_t)pc;
    while (n) {
        sum += (seq_t)(n % 5);
        n /= 5;
    }

    return sum;
}
