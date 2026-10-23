/*
 * A043409  Numbers whose base-7 representation contains exactly one 4.
 * https://oeis.org/A043409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer whose base‑7 representation
 *            contains exactly one digit 4
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   single4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/single4.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t cnt = 0;
    for (;;) {
        ++val;                     /* next candidate */
        if (single4(val)) {
            ++cnt;                 /* one more valid term found */
            if (cnt == n)
                return val;       /* desired term reached */
        }
    }
}
