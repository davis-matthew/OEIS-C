/*
 * A045257  Numbers whose base-5 representation contains exactly two 1's and
 *          two 4's.
 * https://oeis.org/A045257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045257: Numbers whose base‑5 representation contains exactly two 1’s and two 4’s.
   a(n) is the n‑th such integer. Computed by a simple linear scan. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Count of numbers found so far */
    idx_t found = 0;
    /* Current integer being tested */
    seq_t val = 1;

    while (true) {
        seq_t x = val;
        int ones = 0, fours = 0;
        /* Count digits 1 and 4 in base‑5 */
        while (x != 0) {
            int d = (int)(x % 5);       /* digit < 5, safe cast to int */
            if (d == 1)  ++ones;
            else if (d == 4) ++fours;
            x /= 5;
        }
        if (ones == 2 && fours == 2) {
            ++found;
            if (found == n) return val;
        }
        ++val;          /* move to next integer */
    }
}
