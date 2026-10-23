/*
 * A039090  Numbers whose base-9 representation has the same number of 1's and
 *          8's.
 * https://oeis.org/A039090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate non‑negative integers in increasing order and
   select those whose base–9 representation contains an equal number
   of digits 1 and 8.  The first such integer is 0, so a(1)=0. */

#include <stdbool.h>

static inline bool good(seq_t x)
{
    int c1 = 0, c8 = 0;
    while (x) {
        int d = (int)(x % 9);   /* base‑9 digit */
        if (d == 1) ++c1;
        else if (d == 8) ++c8;
        x /= 9;
    }
    return c1 == c8;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;

    while (1) {
        if (good(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
