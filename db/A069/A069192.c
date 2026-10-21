/*
 * A069192  Sum of the reversals of the divisors of n.
 * https://oeis.org/A069192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{d|n} reverse(d) (reverse = decimal digit reversal)
 * Range:     u8 n <= 71, u16 n <= 9458, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* reverse the decimal digits of a positive integer x */
static inline uint64_t rev_uint64(idx_t x)
{
    uint64_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1, but handle 0 safely */
    seq_t sum = 0;
    idx_t d = 1;
    while (d * d <= n) {
        if (n % d == 0) {
            /* divisor d */
            sum += (seq_t)rev_uint64(d);
            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                sum += (seq_t)rev_uint64(d2);
            }
        }
        ++d;
    }
    return sum;
}
