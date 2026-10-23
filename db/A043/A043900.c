/*
 * A043900  Numbers k such that 0 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑6 representation of
 *            k contains the adjacent digits 5 and 0, while the representation
 *            of k+1 does not
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_adjacent_5_0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_5_0.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n = 0 */

    idx_t found = 0;
    seq_t k = (seq_t)0;

    while (found < n) {
        if (has_adjacent_5_0(k)) {
            /* need to check k+1 */
            seq_t kp1 = k + (seq_t)1;
            if (!has_adjacent_5_0(kp1)) {
                ++found;
                if (found == n) return k;
            }
        }
        k = k + (seq_t)1;                     /* go to next integer */
    }
    /* control never reaches here */
    return (seq_t)0;
}
