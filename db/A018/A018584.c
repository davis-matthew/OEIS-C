/*
 * A018584  Divisors of 670.
 * https://oeis.org/A018584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 670 in ascending order, offset 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t divisors[8];
    static int initialized = 0;
    if (!initialized) {
        const uint64_t N = 670ULL;
        idx_t pos = 0;
        for (uint64_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divisors[pos++] = (seq_t)i;
                uint64_t j = N / i;
                if (j != i)
                    divisors[pos++] = (seq_t)j;
            }
        }
        /* sort ascending */
        for (idx_t i = 0; i < 8; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        initialized = 1;
    }
    if (n < 1 || n > 8)
        return 0;
    return divisors[n - 1];
}
