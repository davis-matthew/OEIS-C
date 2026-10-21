/*
 * A063775  Number of 4th powers dividing n.
 * https://oeis.org/A063775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = product_{p^e || n} (1+floor(e/4))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but prevent division by zero */

    uint64_t v = (uint64_t)n;
    seq_t result = 1;

    /* trial division up to sqrt(v) */
    for (uint64_t d = 2; d * d <= v; ++d) {
        if (v % d == 0) {
            uint64_t exp = 0;
            while (v % d == 0) {
                v /= d;
                ++exp;
            }
            result = result * (seq_t)(exp / 4 + 1);
        }
    }

    /* if a prime factor remains > 1, its exponent is 1 (floor(1/4)=0) */
    if (v > 1) {
        result = result * (seq_t)(1);  /* multiply by 1, for clarity */
    }

    return result;
}
