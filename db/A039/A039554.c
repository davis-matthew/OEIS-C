/*
 * A039554  Numbers whose base-12 representation has the same nonzero number
 *          of 8's and 10's.
 * https://oeis.org/A039554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑12 representation
 *            contains * the same nonzero number of digit 8's and digit 10's.
 *            The function searches * numbers in increasing order and counts
 *            the required digits
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for abort() */

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) is not defined, but guard against misuse */
        abort();

    idx_t cand   = 0;       /* current candidate in decimal */
    idx_t found  = 0;       /* how many satisfying numbers have been seen */

    while (1) {
        /* look at the next candidate */
        ++cand;

        /* Count 8's and 10's in the base‑12 representation of cand. */
        int cnt8  = 0;
        int cnt10 = 0;
        seq_t t   = cand;   /* work on a seq_t copy of cand */

        if (t != 0) {
            while (t != 0) {
                unsigned d = (unsigned)(t % 12);   /* digit in [0,11] */
                if (d == 8)        ++cnt8;
                else if (d == 10)  ++cnt10;
                t /= 12;
            }
        }

        /* If counts match and are non‑zero, we found a valid term. */
        if (cnt8 == cnt10 && cnt8 > 0) {
            ++found;
            if (found == n)
                return (seq_t)cand;   /* return the nth term */
        }
    }
}
