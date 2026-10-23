/*
 * A039459  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 5's.
 * https://oeis.org/A039459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑11 representation has the same non‑zero number of 2’s and 5’s. */
#include <stdbool.h>

/* Check whether x has an equal non‑zero number of 2’s and 5’s in base‑11. */
static bool matches(seq_t x)
{
    seq_t tmp = x;
    unsigned char c2 = 0, c5 = 0;

    while (tmp > (seq_t)0) {
        seq_t rem = tmp % (seq_t)11;
        unsigned old = (unsigned)rem;
        if (old == 2) ++c2;
        else if (old == 5) ++c5;
        tmp = tmp / (seq_t)11;
    }
    return (c2 > 0) && (c2 == c5);
}

/* Return the n‑th term (for n ≥ 1). */
seq_t a(idx_t n)
{
    static idx_t cur = 0;          /* current index of the last found term */
    static seq_t last = 0;         /* the last term found */

    /* Find terms until the requested index is reached. */
    while (cur < n) {
        /* Search for the next matching number. */
        last = last + 1;
        while (!matches(last)) {
            last = last + 1;
        }
        ++cur;
    }
    return last;
}
