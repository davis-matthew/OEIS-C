/*
 * A043913  Numbers k such that 0 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj_0_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_adj_0_3.h"

/* Sequence A043913: numbers k such that 0 and 3 occur juxtaposed in
   the base‑7 representation of k but not of k+1.
   Computed by a simple brute‑force search. */
seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t k = 1;

    while (true) {
        if (has_adj_0_3(k) && !has_adj_0_3(k + 1)) {
            ++seen;
            if (seen == n)
                return k;
        }
        ++k;
    }
}
