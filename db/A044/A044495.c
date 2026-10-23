/*
 * A044495  Numbers k such that string 2,0 occurs in the base 6 representation
 *          of k but not of k+1.
 * https://oeis.org/A044495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has_20_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence from OEIS A044495: numbers k such that "20" occurs in the base‑6 representation of k but
   does not occur in the representation of k+1.
   The implementation performs a brute‑force search from 1 upward, testing each candidate. */
/* #include <stdbool.h> for the bool type used below. */
#include <stdbool.h>
#include "../utils/has_20_base6.h"

seq_t a(idx_t n)
{
    /* Find the n‑th integer k (n≥1) satisfying the condition. */
    idx_t  found = 0;
    seq_t  k     = 0;          /* starting from 0, will increment before test */

    for (;; ++k) {
        if (has_20_base6(k)) {                /* k contains '20' */
            /* Only proceed if k+1 does NOT contain '20'; */
            if (!has_20_base6((seq_t)(k + 1))) {
                ++found;
                if (found == n)
                    return k;
            }
        }
    }
}
