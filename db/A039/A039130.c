/*
 * A039130  Numbers whose base-10 representation has the same number of 2's
 *          and 5's.
 * https://oeis.org/A039130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th nonnegative integer whose decimal representation
   contains an equal number of 2’s and 5’s. Calculation is performed by brute‑force
   enumeration. */
#include <stdbool.h>
#include "../utils/equal25.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t cand = 0; ; ++cand) {
        if (equal25(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
    }
}
