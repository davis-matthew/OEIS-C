/*
 * A043245  Numbers k such that 3 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_39_93
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k containing "39" or "93", but not in k-1 */
#include <stdbool.h>
#include "../utils/has_39_93.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    bool prev_has = false;          /* has_39_93(0) == false */

    while (count < n) {
        k++;                         /* start at 1 */
        bool cur_has = has_39_93(k);
        if (cur_has && !prev_has) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        prev_has = cur_has;
    }
    /* Unreachable, but keep the compiler happy */
    return (seq_t)k;
}
