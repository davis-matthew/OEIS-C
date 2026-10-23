/*
 * A043119  Numbers k such that 0 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k whose base‑6 representation * contains
 *            the digits 0 and 4 adjacent (in either order) but k−1 does *
 *            not. Computed by a straightforward linear scan
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_0_and_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_0_and_4.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                               /* number of terms found so far */
    seq_t k = 0;                                   /* current integer being examined */

    while (count < n) {
        k++;                                       /* examine next integer */

        if (has_adjacent_0_and_4(k) &&
            !has_adjacent_0_and_4(k - (seq_t)1)) {  /* k has adjacency, k‑1 does not */
            count++;                               /* accept this term */

            if (count == n)                       /* wanted k reached */
                return k;
        }
    }

    return (seq_t)0;                               /* unreachable, defensive return */
}
