/*
 * A018489  Divisors of 500.
 * https://oeis.org/A018489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 500 in ascending order. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[12];
    static bool init = false;
    if (!init) {
        idx_t low[6];
        idx_t high[6];
        idx_t lowc = 0, highc = 0;
        for (idx_t i = 1; i * i <= 500; ++i) {
            if (500 % i == 0) {
                low[lowc++] = i;
                if (i != 500 / i)
                    high[highc++] = 500 / i;
            }
        }
        idx_t k = 0;
        for (idx_t i = 0; i < lowc; ++i) {
            divs[k++] = (seq_t)low[i];
        }
        for (idx_t i = highc; i > 0; --i) {
            divs[k++] = (seq_t)high[i - 1];
        }
        init = true;
    }
    if (n == 0 || n > 12) {
        return 0;
    }
    return divs[n - 1];
}
