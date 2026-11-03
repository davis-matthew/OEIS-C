/*
 * A039406  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 6's.
 * https://oeis.org/A039406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039406: numbers whose decimal representation has the same
   nonzero number of 1's and 6's. The function returns the n-th term
   (offset 1), computed by brute‑force enumeration. */

#include <stdbool.h>

/* Return true if x has at least one digit 1 and one digit 6 and
   the counts are equal. */
static bool has_equal_nonzero_ones_and_sixes(seq_t x)
{
    unsigned int c1 = 0, c6 = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned int d = (unsigned)(t % 10);
        if (d == 1)
            ++c1;
        else if (d == 6)
            ++c6;
        t /= 10;
    }
    return c1 > 0 && c6 > 0 && c1 == c6;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but guard against misuse */
        return 0;
    idx_t found = 0;
    seq_t val = 0;
    while (true) {
        ++val;
        if (has_equal_nonzero_ones_and_sixes(val)) {
            ++found;
            if (found == n)
                return val;
        }
    }
}
