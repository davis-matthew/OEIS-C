/*
 * A093061  6 * Sum_{d|n} (d mod 3).
 * https://oeis.org/A093061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * Σ_{d|n} (d mod 3)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n == 0 we simply return 0; the OEIS offset starts at 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t i;

    /* Enumerate all divisors by checking up to sqrt(n). */
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* d = i */
            seq_t mod1 = (seq_t)(i % 3);
            sum += mod1;

            idx_t j = n / i;          /* paired divisor */
            if (j != i) {
                seq_t mod2 = (seq_t)(j % 3);
                sum += mod2;
            }
        }
    }
    return sum * (seq_t)6;
}
