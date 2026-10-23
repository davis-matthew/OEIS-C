/*
 * A043918  Numbers k such that 1 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by scanning for the nth k such that its base‑7 *
 *            representation contains the digits 1 and 3 adjacent (in either
 *            order), * while the base‑7 representation of k+1 does not
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent13.h"

/* External-linkage function that returns the nth term (1‑based) of the
 * sequence A043918. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, harmless */

    seq_t k = 0;
    idx_t count = 0;
    while (true) {
        if (has_adjacent13(k) && !has_adjacent13(k + (seq_t)1)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
