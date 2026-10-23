/*
 * A043378  Numbers having two 2's in base 6.
 * https://oeis.org/A043378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043378: numbers whose base‑6 representation contains exactly two digits equal to 2 */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* a(0) is undefined; return 0 */

    idx_t seen = 0;
    idx_t x = 0;

    while (1) {
        /* Count digits equal to 2 in base 6 */
        idx_t y = x;
        int cnt2 = 0;
        if (y == 0) {
            /* number 0 has 0 twos */
        } else {
            while (y > 0) {
                if ((y % 6ULL) == 2) {
                    cnt2++;
                    if (cnt2 > 2) break;   /* early exit */
                }
                y /= 6ULL;
            }
        }

        if (cnt2 == 2) {
            seen++;
            if (seen == n) {
                return (seq_t)x;
            }
        }
        x++;
    }
}
