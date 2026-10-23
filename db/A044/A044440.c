/*
 * A044440  Numbers n such that string 2,1 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A044440
   Numbers n such that the base‑3 representation of n contains the
   digit sequence "21", but the base‑3 representation of n+1 does not.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/has21.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, so n>=1 expected */

    idx_t found = 0;
    seq_t candidate = 0;

    /* Sequentially search for numbers with the required property. */
    while (found < n) {
        ++candidate;
        if (has21(candidate) && !has21(candidate + 1)) {
            ++found;
        }
    }
    return candidate;
}
