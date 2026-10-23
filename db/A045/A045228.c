/*
 * A045228  Numbers whose base-5 representation contains exactly two 1's and
 *          three 2's.
 * https://oeis.org/A045228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation contains exactly two 1's and three 2's.
   Enumerate the terms in increasing order.  The function simply scans
   successive integers, testing the digit condition, and stops when the
   n‑th such integer has been found. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Guard against the 0th index (offset is 1). */
    if (n == 0) return (seq_t)0;

    seq_t current = (seq_t)0;
    idx_t found  = 0;

    for (;;) {
        /* Advance to the next integer. */
        current = current + (seq_t)1;

        /* Count the number of 1's and 2's in the base‑5 representation. */
        seq_t tmp = current;
        int c1 = 0;
        int c2 = 0;
        while (tmp > (seq_t)0) {
            int digit = (int)(tmp % (seq_t)5);
            if (digit == 1)       c1++;
            else if (digit == 2)  c2++;
            tmp = tmp / (seq_t)5;
        }

        if (c1 == 2 && c2 == 3) {
            ++found;
            if (found == n) return current;
        }
    }
}
