/*
 * A039155  Numbers whose base-10 representation has the same number of 8's
 *          and 9's.
 * https://oeis.org/A039155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose decimal
 *            representation contains the same number of digits 8 and 9. a(1)
 *            = 0, a(2) = 1, a(3) = 2, …
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_8_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_8_9.h"

/* External‑linkage function requested by the build. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand  = 0;

    while (count < n) {
        if (equal_8_9(cand))
            count++;
        if (count == n)
            return cand;
        cand = cand + 1;
    }
    /* unreachable, but keep compiler happy */
    return cand;
}
