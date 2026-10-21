/*
 * A069978  (Sum of digits of n)^4 - (sum of digits of n^4).
 * https://oeis.org/A069978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (digitSum(n))^4 - digitSum(n^4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of decimal digits of a value of type seq_t. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* n as seq_t */
    seq_t s = digit_sum(sn);       /* sum of digits of n */

    /* Compute n^4 = (n * n) * (n * n) */
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;

    /* Compute s^4 = (s*s) * (s*s) */
    seq_t s2 = s * s;
    seq_t s4 = s2 * s2;

    seq_t s4n4 = digit_sum(n4);    /* sum of digits of n^4 */

    return s4 - s4n4;
}
