/*
 * A039439  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 2's.
 * https://oeis.org/A039439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-11 representation has the same nonzero number of 0's and 2's. */
#include <stdbool.h>

static inline bool good_value(seq_t v)
{
    int zeros = 0, twos = 0;
    while (v > 0) {
        seq_t d = v % 11;
        if (d == 0)
            ++zeros;
        else if (d == 2)
            ++twos;
        v /= 11;
    }
    return zeros > 0 && zeros == twos;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found    = 0;

    while (found < n) {
        ++candidate;
        if (good_value(candidate))
            ++found;
    }

    return candidate;
}
