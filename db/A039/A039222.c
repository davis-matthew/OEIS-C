/*
 * A039222  Numbers whose base-12 representation has the same number of 1's
 *          and 4's.
 * https://oeis.org/A039222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039222: numbers whose base-12 representation has the same number of 1's and 4's, computed by enumeration. */

#include <stddef.h>

static int is_good(seq_t x) {
    seq_t t = x;
    unsigned int ones = 0, fours = 0;
    /* Count digits of x in base 12 */
    while (t > 0) {
        unsigned int d = (unsigned int)(t % (seq_t)12);
        if (d == 1) ++ones;
        else if (d == 4) ++fours;
        t /= (seq_t)12;
    }
    return ones == fours;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t val = (seq_t)0;
    for (;;) {
        if (is_good(val)) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
}
