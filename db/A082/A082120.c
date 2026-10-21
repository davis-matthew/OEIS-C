/*
 * A082120  Smallest difference > 1 between d and n/d for any divisor d of n.
 * https://oeis.org/A082120
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{d|n, d ≤ n/d, (n/d – d) > 1} (n/d – d)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    // For n < 3 the value is irrelevant; return 0.
    if (n < 3)
        return 0;

    seq_t best = 0;
    bool found = false;

    /* iterate over all divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t q  = n / i;               /* complementary divisor */
            seq_t diff = (seq_t)q - (seq_t)i; /* n/d - d, guaranteed ≥ 0 */

            if (diff > 1) {
                if (!found || diff < best) {
                    best = diff;
                    found = true;
                }
            }
        }
    }

    /* For any n ≥ 3 a divisor pair exists with diff > 1 (e.g., d = 1). */
    return best;
}
