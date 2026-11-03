/*
 * A039541  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 9's.
 * https://oeis.org/A039541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose base‑12 representation contains the
 *            same nonzero number of 5’s and 9’s. The function scans integers
 *            sequentially and counts those that satisfy this property
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_equal_nonzero_5_and_9(seq_t v)
{
    unsigned int c5 = 0, c9 = 0;
    seq_t x = v;
    if (x == 0)
        return false;
    while (x > 0) {
        seq_t d = x % (seq_t)12;
        if (d == (seq_t)5) ++c5;
        else if (d == (seq_t)9) ++c9;
        x = x / (seq_t)12;
    }
    return c5 > 0 && c5 == c9;
}

seq_t a(idx_t n)
{
    seq_t found = 0;
    seq_t count = 0;
    seq_t candidate = 1;

    while (true) {
        if (has_equal_nonzero_5_and_9(candidate)) {
            ++count;
            if (count == n) {
                found = candidate;
                break;
            }
        }
        ++candidate;
    }
    return found;
}
