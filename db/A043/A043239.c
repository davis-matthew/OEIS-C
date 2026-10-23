/*
 * A043239  Numbers k such that 2 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_29_or_92
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth integer k such that the base‑10 representation of k contains the substring "29" or "92", but the representation of k‑1 does not. */

#include <stdbool.h>
#include "../utils/has_29_or_92.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 */

    seq_t k = 1;
    idx_t count = 0;
    while (1) {
        if (has_29_or_92(k) && !has_29_or_92(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
