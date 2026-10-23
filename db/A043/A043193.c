/*
 * A043193  Numbers k such that 1 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_17_or_71
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043193:
   a(n) = the nth integer k ≥ 1 such that in the base‑9 representation
   of k the digits 1 and 7 appear consecutively (either "17" or "71"),
   while the base‑9 representation of k − 1 does not contain such
   a pair. */
#include <stdbool.h>
#include "../utils/has_17_or_71.h"

seq_t a(idx_t n)
{
    seq_t k = 1;            /* candidate k */
    idx_t count = 0;        /* number of qualifying k's found */

    for (;;) {
        if (has_17_or_71(k)) {
            seq_t km1 = k - (seq_t)1;
            if (!has_17_or_71(km1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
}
