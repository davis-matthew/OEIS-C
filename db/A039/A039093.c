/*
 * A039093  Numbers whose base-9 representation has the same number of 2's and
 *          5's.
 * https://oeis.org/A039093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039093: Numbers whose base‑9 representation has the same number
   of 2's and 5's.
   Implementation scans integers increasing from 0 until the desired
   candidate is found.  The function runs in O(m) time where m is the
   value of the n‑th term (≤ ≈ 10⁶ for n ≤ 10 000). */

#include <stdbool.h>
#include "../utils/has_equal_25.h"

/* External entry point requested by the build system */
seq_t a(idx_t n) {
    idx_t found = 0;          /* how many valid numbers we have seen */
    seq_t cur   = (seq_t)0;   /* current integer to test */

    /* Scan upwards until the n-th valid number is found. */
    while (1) {
        if (has_equal_25(cur)) {
            ++found;
            if (found == n) {
                return cur;
            }
        }
        ++cur;
    }
}
