/*
 * A039290  Numbers whose base-5 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A039290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains the same positive number of digits 2 and 3.
 *            Computation: simple linear search enumerating integers until the
 *            n‑th valid one is found
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdbool.h>
#endif

static bool has_equal_23(seq_t x)
{
    seq_t t = x;
    unsigned int c2 = 0, c3 = 0;
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 5);
        if (d == 2) ++c2;
        else if (d == 3) ++c3;
        t = t / 5;
    }
    return (c2 == c3 && c2 > 0);
}

seq_t a(idx_t n)
{
    /* Off‑set 1: a(1) is the first term.  a(0) is undefined. */
    if (n == 0) return 0;

    seq_t val = 1;
    idx_t count = 0;
    while (count < n) {
        if (has_equal_23(val)) {
            ++count;
            if (count == n) return val;
        }
        val = val + 1;
    }
    return 0; /* unreachable */
}
