/*
 * A043903  Numbers k such that 1 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the base‑6 representation of
 *            k contains adjacent digits 1 and 4 (in either order) but the
 *            base‑6 representation of k + 1 does not. The sequence starts 10,
 *            25, 46, 65, 82, 97,…
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adjacent_14.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  n >= 1.  We search for
       successive integers k satisfying the condition.  For the
       expected test sizes (n up to about 10 000) a simple linear
       search with the auxiliary has_adjacent_14 function is fast
       enough and keeps the code short and portable. */
    uint64_t count = 0;
    uint64_t k = 0;

    for (;;) {
        ++k;
        if (has_adjacent_14(k) && !has_adjacent_14(k + 1U)) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)k;
        }
    }
}
