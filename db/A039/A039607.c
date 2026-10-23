/*
 * A039607  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 5's.
 * https://oeis.org/A039607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A039607:
   the nth positive integer whose base‑12 representation
   contains an equal, non‑zero number of digits 1 and 5.
   This is implemented with a simple brute‑force search,
   which is fast enough for the required N ≤ 10 000. */

#include <stdbool.h>

/* Test whether a 64‑bit integer has an equal non‑zero number
   of digits 1 and 5 in base‑12. */
static bool good(idx_t x)
{
    unsigned c1 = 0, c5 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 12u);
        if (d == 1u) ++c1;
        else if (d == 5u) ++c5;
        x /= 12u;
    }
    return c1 > 0 && c1 == c5;
}

seq_t a(idx_t n)
{
    idx_t candidate = 1;
    idx_t found = 0;

    while (true) {
        if (good(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
