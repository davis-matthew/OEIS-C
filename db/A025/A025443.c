/*
 * A025443  Number of partitions of n into 4 distinct nonzero squares.
 * https://oeis.org/A025443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = [x^n y^4] Product_{k≥1} (1 + y*x^(k²)) Number of
 *            partitions of n into 4 distinct non‑zero squares
 * Range:     u8 n <= 4589, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline idx_t int_isqrt(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    idx_t limit = int_isqrt(n);
    for (idx_t i = 1; i + 3 <= limit; ++i) {
        idx_t i2 = i * i;
        for (idx_t j = i + 1; j + 2 <= limit; ++j) {
            idx_t j2 = j * j;
            for (idx_t k = j + 1; k + 1 <= limit; ++k) {
                idx_t k2 = k * k;
                for (idx_t l = k + 1; l <= limit; ++l) {
                    idx_t sum = i2 + j2 + k2 + l * l;
                    if (sum > n) break;          /* larger l only increases sum */
                    if (sum == n) ++count;       /* count this partition */
                }
            }
        }
    }
    return count;
}
