/*
 * A166250  a(n) = n-1 plus the largest proper divisor of n.
 * https://oeis.org/A166250
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + largest proper divisor of n (which equals n /
 *            spf(n))
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* index below the offset, value irrelevant */
    idx_t spf = 0;
    if ((n & 1ULL) == 0) {
        spf = 2;
    } else {
        for (idx_t i = 3; i <= n / i; i += 2) {
            if (n % i == 0) {
                spf = i;
                break;
            }
        }
        if (spf == 0) spf = n;  /* n is prime */
    }
    seq_t divisor = (seq_t)n / (seq_t)spf;
    return (seq_t)n - 1 + divisor;
}
