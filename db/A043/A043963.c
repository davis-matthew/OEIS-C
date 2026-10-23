/*
 * A043963  Numbers k such that 0 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043963: numbers k such that 0 and 4 occur juxtaposed
   in the base‑9 representation of k but not of k+1 */

#include <stddef.h>

/* Return 1 if x in base‑9 has adjacent digits 0 and 4 (in either order) */
static int has_adjacent_04(seq_t x)
{
    if (x < 9)
        return 0;
    seq_t prev = x % 9;
    x /= 9;
    while (x > 0) {
        seq_t cur = x % 9;
        if ((prev == 0 && cur == 4) || (prev == 4 && cur == 0))
            return 1;
        prev = cur;
        x /= 9;
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t k = 0;
    while (count < n) {
        if (has_adjacent_04(k) && !has_adjacent_04(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    return 0; /* Unreachable */
}
