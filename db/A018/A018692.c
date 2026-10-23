/*
 * A018692  Divisors of 868.
 * https://oeis.org/A018692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 868 in ascending order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for the divisors of 868 */
    static seq_t divisors[12];
    static size_t div_count = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t N = 868;
        seq_t lower[12];
        seq_t upper[12];
        size_t lc = 0, uc = 0;

        for (idx_t d = 1; d * d <= N; ++d) {
            if (N % d == 0) {
                lower[lc++] = (seq_t)d;
                if (d * d != N) {
                    upper[uc++] = (seq_t)(N / d);
                }
            }
        }

        /* copy lower part (already sorted) */
        for (size_t i = 0; i < lc; ++i) {
            divisors[i] = lower[i];
        }
        /* copy upper part in reverse order to get ascending order */
        for (size_t j = 0; j < uc; ++j) {
            divisors[lc + j] = upper[uc - 1 - j];
        }
        div_count = lc + uc;
        initialized = true;
    }

    if (n == 0 || n > div_count) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
