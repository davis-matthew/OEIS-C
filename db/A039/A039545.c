/*
 * A039545  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 8's.
 * https://oeis.org/A039545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   base12_has_equal_nonzero_6_and_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Scan integers in increasing order and select those whose
 * base‑12 representation contains the same non‑zero number of 6's and 8's.  */
/* The function returns the nth such number. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/base12_has_equal_nonzero_6_and_8.h"

seq_t a(idx_t n)
{
    /* n is 1‑based */
    seq_t cur = (seq_t)80;           /* first candidate with one 6 and one 8 */
    idx_t seen = 0;
    while (seen < n) {
        if (base12_has_equal_nonzero_6_and_8(cur)) {
            seen++;
            if (seen == n) return cur;
        }
        cur++;
    }
    /* unreachable */
    return (seq_t)0;
}
