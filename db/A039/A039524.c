/*
 * A039524  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 5's.
 * https://oeis.org/A039524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑12 representation contains the
   same non‑zero number of digits 3 and 5. This is done by a linear scan
   counting valid numbers.  */

#include <stdbool.h>

static bool ok_uint64(uint64_t x)
{
    int c3 = 0, c5 = 0;
    while (x > 0) {
        int d = (int)(x % 12);
        if (d == 3) ++c3;
        if (d == 5) ++c5;
        x /= 12;
    }
    return c3 == c5 && c3 > 0;
}

seq_t a(idx_t n)
{
    uint64_t cur = 0;      /* current integer being tested */
    idx_t   found = 0;     /* how many valid numbers found so far */

    while (found < n) {
        ++cur;
        if (ok_uint64(cur)) {
            ++found;
        }
    }
    return (seq_t)cur;
}
