/*
 * A039110  Numbers whose base-9 representation has the same number of 6's and
 *          8's.
 * https://oeis.org/A039110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A039110:
   Numbers whose base‑9 representation contains an equal number of 6's and 8's.
   The function returns the n‑th such number (1‑based indexing, a(1)=0). */

#include <stdbool.h>          /* for bool */
#include "../utils/balanced_in_base9.h"

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* safety; offset starts at 1 */
    }

    idx_t count = 0;
    for (idx_t val = 0; ; ++val) {
        if (balanced_in_base9(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;
            }
        }
    }
}
