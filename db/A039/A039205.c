/*
 * A039205  Numbers whose base-11 representation has the same number of 7's
 *          and 9's.
 * https://oeis.org/A039205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_7_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_7_9.h"

seq_t a(idx_t n)
{
    seq_t cand  = 0;
    idx_t count = 0;

    while (true) {
        if (equal_7_9(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
