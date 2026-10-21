/*
 * A083728  a(0) = 1, a(n) = 480*sigma(n).
 * https://oeis.org/A083728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 480 * sigma(n)
 * Range:     u8 n <= 0, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;            /* a(0) = 1 */
    }

    seq_t sigma = 0;
    for (idx_t i = 1; i <= n / i; ++i) {   /* iterate over divisor pairs */
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i) {
                sigma += (seq_t)j;
            }
        }
    }

    return (seq_t)480 * sigma;   /* a(n) = 480 * sigma(n) */
}
