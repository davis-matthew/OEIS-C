/*
 * A001995  Numbers that are the sum of 5 distinct squares: of form v^2 + w^2
 *          + x^2 + y^2 + z^2 with 0 <= v < w < x < y < z.
 * https://oeis.org/A001995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every v^2+w^2+x^2+y^2+z^2 with 0 <= v < w < x < y
 *            < z up to a bound (doubled until enough terms), then return the
 *            n-th marked number
 * Range:     u8 n <= 179, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t limit = (uint64_t)n + 1000;

    for (;;) {
        unsigned char *is_sum = (unsigned char *)malloc((size_t)(limit + 1));
        memset(is_sum, 0, (size_t)(limit + 1));

        for (uint64_t z = 4; z * z <= limit; z++) {
            uint64_t z2 = z * z;
            for (uint64_t y = 3; y < z && y * y + z2 <= limit; y++) {
                uint64_t s2 = y * y + z2;
                for (uint64_t x = 2; x < y && x * x + s2 <= limit; x++) {
                    uint64_t s3 = x * x + s2;
                    for (uint64_t w = 1; w < x && w * w + s3 <= limit; w++) {
                        uint64_t s4 = w * w + s3;
                        for (uint64_t v = 0; v < w; v++) {
                            uint64_t s = v * v + s4;
                            if (s > limit) break;
                            is_sum[s] = 1;
                        }
                    }
                }
            }
        }

        idx_t count = 0;
        for (uint64_t i = 0; i <= limit; i++) {
            if (is_sum[i]) {
                count++;
                if (count == n) {
                    free(is_sum);
                    return (seq_t)i;
                }
            }
        }

        free(is_sum);
        limit *= 2;
    }
}
