/*
 * A039474  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 7's.
 * https://oeis.org/A039474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base-11 representation contains
 *            the same non‑zero number of digits 4 and 7. a(1)=51. The
 *            function scans successive integers starting from 0, counts the
 *            4’s and 7’s in base 11, and returns the n‑th number that
 *            satisfies the equality
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_equal_4_7_base11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/has_equal_4_7_base11.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = 0;

    while (1) {
        if (has_equal_4_7_base11(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;
    }
}
