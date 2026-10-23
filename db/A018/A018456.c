/*
 * A018456  Divisors of 438.
 * https://oeis.org/A018456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Divisors of 438 in ascending order. */

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t divisors[8];

    if (!ready) {
        const idx_t full = 438;
        size_t cnt = 0;
        for (idx_t d = 1; d <= full; ++d) {
            if (full % d == 0) {
                divisors[cnt++] = (seq_t)d;
            }
        }
        ready = true;
    }

    if (n == 0) return 0;              /* undefined for n < 1 */
    if (n <= 8) return divisors[(size_t)(n - 1)];
    return 0;                           /* no more terms */
}
