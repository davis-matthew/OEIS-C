/*
 * A018772  Divisors of 1008.
 * https://oeis.org/A018772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 1008 in ascending order
 * Range:     u8 n <= 21, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* 1008 = 2^4 * 3^2 * 7^1  -> 30 divisors */
    static bool init = false;
    static seq_t divisors[30];

    if (!init) {
        seq_t p2[5], p3[3], p7[2];
        p2[0] = 1; for (idx_t i = 1; i < 5; ++i) p2[i] = p2[i-1] * 2;
        p3[0] = 1; for (idx_t j = 1; j < 3; ++j) p3[j] = p3[j-1] * 3;
        p7[0] = 1; for (idx_t k = 1; k < 2; ++k) p7[k] = p7[k-1] * 7;

        idx_t idx = 0;
        for (idx_t i = 0; i < 5; ++i)
            for (idx_t j = 0; j < 3; ++j)
                for (idx_t k = 0; k < 2; ++k)
                    divisors[idx++] = p2[i] * p3[j] * p7[k];

        /* simple insertion sort */
        for (idx_t i = 1; i < 30; ++i) {
            seq_t key = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j-1] > key) {
                divisors[j] = divisors[j-1];
                --j;
            }
            divisors[j] = key;
        }
        init = true;
    }

    if (n == 0) return 0;
    if (n > 30)  return 0;
    return divisors[n-1];
}
