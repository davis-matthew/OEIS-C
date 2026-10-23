/*
 * A182695  a(n) = the smallest 4-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest 4‑digit number having exactly n divisors; a(n) =
 *            0 if no such number exists
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* At most 64 divisors for any 4‑digit integer. */
    static seq_t best[65] = {0};
    static bool built = false;

    if (!built) {
        for (idx_t m = 1000; m <= 9999; ++m) {
            /* Count divisors of m. */
            idx_t d = 0;
            for (idx_t i = 1; i * i <= m; ++i) {
                if (m % i == 0) {
                    ++d;
                    if (i != m / i) ++d;
                }
            }
            if (d <= 64) {
                unsigned int id = (unsigned int)d;
                if (best[id] == 0 || (seq_t)m < best[id]) {
                    best[id] = (seq_t)m;
                }
            }
        }
        built = true;
    }

    if (n < 1 || n > 64) return 0;
    return best[(unsigned int)n];
}
