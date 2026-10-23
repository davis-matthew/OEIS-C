/*
 * A043942  Numbers k such that 1 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent15_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043942:  numbers k such that 1 and 5 occur juxtaposed in the
   base‑8 representation of k but not of k+1.
   The function returns the n‑th term (a(1)=13, a(2)=41, …).  It
   first tests for adjacency of 1 and 5 in either order. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adjacent15_oct.h"

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1: n==0 is undefined */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k     = 0;

    for (;;) {
        if (has_adjacent15_oct(k) && !has_adjacent15_oct(k + 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
