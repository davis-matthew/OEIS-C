/*
 * A039476  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 9's.
 * https://oeis.org/A039476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-11 representation has the same nonzero number of 4’s and 9’s. */
#include <stdint.h>   /* for uint64_t used by idx_t, just in case */

/* This implementation scans the positive integers, counts the 4’s and 9’s
   in their base‑11 representation and returns the nth number that satisfies
   the condition.  It works for all allowed seq_t (unsigned integer types
   or arbitrary‑precision classes) and for all n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1, so we return 0 for 0 to keep the function safe */
        return 0;

    idx_t found = 0;          /* how many qualifying numbers we have seen   */
    seq_t x = 1;              /* current integer to test                    */

    for (;; ++x) {
        seq_t t = x;
        unsigned int cnt4 = 0, cnt9 = 0;

        /* count 4’s and 9’s in the base‑11 representation of x */
        while (t > 0) {
            unsigned int d = (unsigned int)(t % 11);
            if (d == 4) ++cnt4;
            else if (d == 9) ++cnt9;
            t /= 11;
        }

        if (cnt4 == cnt9 && cnt4 > 0) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
