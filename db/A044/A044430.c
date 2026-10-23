/*
 * A044430  Numbers n such that string 9,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has98
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iteratively search integers until the n‑th number that
   contains the decimal substring "98" while the preceding integer does
   not.  This yields OEIS A044430. */

#include <stdbool.h>
#include "../utils/has98.h"

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0, placeholder */

    idx_t  found = 0;
    seq_t  cur   = 1;

    while (true) {
        if (has98(cur)) {
            seq_t prev = cur;
            --prev;                   /* compute cur - 1 */
            if (!has98(prev)) {
                ++found;
                if (found == n)
                    return cur;
            }
        }
        ++cur;
    }
}
