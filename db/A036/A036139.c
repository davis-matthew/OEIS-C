/*
 * A036139  a(n) = 5^n mod 103.
 * https://oeis.org/A036139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 103
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t period[102];
    static bool initialized = false;

    if (!initialized) {
        period[0] = 1;
        for (idx_t i = 1; i < 102; ++i) {
            period[i] = (period[i-1] * (seq_t)5) % (seq_t)103;
        }
        initialized = true;
    }

    return period[n % 102];
}
