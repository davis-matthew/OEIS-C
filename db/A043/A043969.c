/*
 * A043969  Numbers k such that 1 and 3 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_13_or_31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) : number k such that the base‑9 representation of k
 *  contains the consecutive digits “13” or “31” but the
 *  base‑9 representation of k+1 does not contain either
 *  (OEIS A043969).  The sequence starts 12, 28, 93, 116, …
 */

#include <stdbool.h>
#include "../utils/has_adjacent_13_or_31.h"

seq_t a(idx_t n)
{
    seq_t k = 0;
    idx_t found = 0;

    for (;;) {
        if (has_adjacent_13_or_31(k) && !has_adjacent_13_or_31(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
