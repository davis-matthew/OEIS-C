/*
 * A043183  Numbers k such that 0 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 0 and 4 occur juxtaposed in base‑9 *
 *            representation of k but not of k-1
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;                /* start checking from 1 (k-1 = 0) */
    bool prev_adj = false;      /* base‑9 of 0 has no adjacency */

    while (1) {
        /* test whether k has an adjacent 0–4 or 4–0 pair in base 9 */
        seq_t x = k;
        bool adj = false;
        if (x >= 9) {
            seq_t prev_d = x % 9;
            x /= 9;
            while (x > 0) {
                seq_t cur_d = x % 9;
                if ((prev_d == 0 && cur_d == 4) ||
                    (prev_d == 4 && cur_d == 0)) {
                    adj = true;
                    break;
                }
                prev_d = cur_d;
                x /= 9;
            }
        }

        if (adj && !prev_adj) {
            ++count;
            if (count == n)
                return k;
        }

        prev_adj = adj;
        ++k;
    }
}
