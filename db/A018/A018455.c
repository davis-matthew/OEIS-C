/*
 * A018455  Divisors of 436.
 * https://oeis.org/A018455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 436, listed in increasing order. The
 *            divisors of 436 are 1, 2, 4, 109, 218, 436. For n > 6 the
 *            function returns 0
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    static seq_t divisors[6];
    static bool initialized = false;
    if (!initialized) {
        idx_t m = 436;
        idx_t count = 0;
        for (idx_t i = 1; i <= m; ++i) {
            if (m % i == 0) {
                divisors[count++] = (seq_t)i;
            }
        }
        initialized = true;
    }
    if (n <= 6) {
        return divisors[n - 1];
    }
    return (seq_t)0;
}
