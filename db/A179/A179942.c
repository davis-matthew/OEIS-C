/*
 * A179942  Number of times n appears in a 1000 X 1000 multiplication table.
 * https://oeis.org/A179942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, less
 * Formula:   * a(n) = number of divisors d of n such that d <= 1000 and n/d
 *            <= 1000. * For n > 1e6, a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    const idx_t MAX_N    = 1000000ULL;   /* 1000 * 1000 */
    const idx_t LIMIT    = 1000ULL;

    if (n > MAX_N) {
        return 0;
    }

    /* ceil division: smallest divisor that can pair with a value <= 1000 */
    idx_t L = (n + LIMIT - 1) / LIMIT;

    seq_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            if (d >= L && d <= LIMIT) {
                ++count;
            }
            if (e != d && e >= L && e <= LIMIT) {
                ++count;
            }
        }
    }

    return count;
}
