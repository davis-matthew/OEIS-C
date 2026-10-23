/*
 * A044015  Numbers k such that 2 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer k whose decimal representation contains
   the adjacent digits 2 and 5 (in either order) while k+1 does not. */

#include <stdbool.h>

static bool has25or52(seq_t x)
{
    if (x < 10)
        return false;
    while (x >= 10) {
        seq_t d1 = x % 10;
        x = x / 10;
        seq_t d0 = x % 10;
        if ((d0 == 2 && d1 == 5) || (d0 == 5 && d1 == 2))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; this case never occurs */

    seq_t k = 1;
    idx_t found = 0;
    while (1) {
        if (has25or52(k)) {
            seq_t kp1 = k + 1;
            if (!has25or52(kp1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        k = k + 1;
    }
}
