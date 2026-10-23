/*
 * A146099  Bell numbers (A000110) read mod 9.
 * https://oeis.org/A146099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell number B_n modulo 9. a(n+39) = a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A146099: Bell numbers read mod 9. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t bell[39];
    static bool init = false;

    if (!init) {
        bell[0] = 1 % 9;          /* B_0 = 1 */

        for (idx_t i = 0; i < 38; ++i) {
            uint64_t sum = 0;
            uint64_t binom = 1;

            for (idx_t k = 0; k <= i; ++k) {
                if (k > 0)
                    binom = binom * (i - k + 1) / k;  /* C(i, k) */

                sum = (sum + (binom % 9ULL) * (unsigned long long)bell[k]) % 9ULL;
            }

            bell[i + 1] = (seq_t)sum;
        }

        init = true;
    }

    return bell[n % 39];
}
