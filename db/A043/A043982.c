/*
 * A043982  Numbers k such that 3 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent35
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043982
 * Numbers k such that the base‑9 representation of k contains the
 * adjacent digits 3 and 5 in either order, but the representation of
 * k+1 does not.
 * This function computes the nth term (n ≥ 1) by brute‑force search.
 * It has no external dependencies apart from <stdbool.h>. */

#include <stdbool.h>
#include "../utils/has_adjacent35.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* harmless when n==0 (offset is 1) */

    idx_t found = 0;
    idx_t k = 1;
    for (;;) {
        if (has_adjacent35(k) && !has_adjacent35(k + 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
        ++k;
    }
}
