/*
 * A039590  Numbers whose base-6 representation has the same number of 2's,
 *          3's and 5's.
 * https://oeis.org/A039590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer whose base‑6 representation
 *            * contains an equal number of digits 2, 3 and 5
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has the same number of 2's, 3's and 5's in base 6. */
static inline bool good(seq_t x)
{
    unsigned int c2 = 0, c3 = 0, c5 = 0;

    /* The representation of 0 is "0", containing zero 2's, 3's and 5's. */
    if (x == 0) return true;

    while (x > 0) {
        unsigned int d = (unsigned int)(x % 6);
        if (d == 2) ++c2;
        else if (d == 3) ++c3;
        else if (d == 5) ++c5;
        x /= 6;
    }
    return (c2 == c3 && c3 == c5);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, although n ≥ 1 in the spec */
    seq_t target = (seq_t)n;       /* convert the index to the seq_t type      */
    seq_t cand   = 0;
    seq_t found  = 0;

    while (found < target) {
        if (good(cand)) {
            ++found;
            if (found == target) return cand;
        }
        ++cand;
    }
    /* The loop always returns; this line is only to satisfy the compiler. */
    return cand;
}
