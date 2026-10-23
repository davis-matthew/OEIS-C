/*
 * A028438  Golc sequence in base 8. Left to right concatenation of
 *          n,int(log_8(n)),int(log_8(int(log_8(n)))),... in base8.
 * https://oeis.org/A028438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_log8_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation in base‑8 of
 * n, floor(log_8(n)), floor(log_8(floor(log_8(n)))), …,
 * stopping after the first term that is < 2.
 * The function returns the resulting integer interpreted as a
 * normal (base‑10) value.
 */

#include <stdint.h>
#include "../utils/_floor_log8_u64.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;          /* start with the base‑8 value of n */
    idx_t cur = _floor_log8_u64(n);   /* first log term */

    while (1) {
        /* find number of base‑8 digits of cur */
        int len = 0;
        idx_t tmp = cur;
        if (tmp == 0) {
            len = 1;
        } else {
            while (tmp > 0) {
                ++len;
                tmp /= 8;
            }
        }

        /* multiply result by 8^len */
        seq_t mult = 1;
        for (int i = 0; i < len; ++i) mult *= (seq_t)8;
        result = result * mult + (seq_t)cur;

        if (cur < 2) break;          /* stop after the first term < 2 */
        cur = _floor_log8_u64(cur);  /* prepare next log term */
    }
    return result;
}
