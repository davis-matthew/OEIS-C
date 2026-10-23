/*
 * A018539  Divisors of 596.
 * https://oeis.org/A018539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 596 in ascending order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[128];
    static size_t cnt = 0;
    static bool init = false;
    const idx_t N = 596;

    if (!init) {
        /* Discover the divisors of 596 */
        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divs[cnt++] = (seq_t)i;
                if (i != N / i) {
                    divs[cnt++] = (seq_t)(N / i);
                }
            }
        }
        /* Sort the divisors in ascending order */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t t = divs[i];
                    divs[i] = divs[j];
                    divs[j] = t;
                }
            }
        }
        init = true;
    }

    if (n == 0) return 0;
    if ((idx_t)n <= cnt) {
        return divs[n - 1];
    }
    return 0;
}
