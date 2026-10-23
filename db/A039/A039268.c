/*
 * A039268  Numbers whose base-12 representation has the same number of 7's
 *          and 11's.
 * https://oeis.org/A039268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑12 representation
 *            has an equal number of digits 7 and 11
 * Range:     u8 n <= 187, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   equal_7_and_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_7_and_11.h"

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t v = 0; found < n; ++v) {
        if (equal_7_and_11(v)) {
            ++found;
            if (found == n) {
                return v;
            }
        }
    }
    /* The loop always returns before reaching this point. */
    return 0;
}
