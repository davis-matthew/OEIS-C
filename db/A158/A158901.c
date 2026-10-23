/*
 * A158901  A051731 * (1, 1, 2, 3, 4, 5, ...).
 * https://oeis.org/A158901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - d(n) + 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sigma = 0;
    idx_t d = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            ++d;
            idx_t j = n / i;
            if (j != i) {
                sigma += (seq_t)j;
                ++d;
            }
        }
    }
    return sigma - (seq_t)d + 1;
}
