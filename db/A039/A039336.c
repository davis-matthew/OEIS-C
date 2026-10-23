/*
 * A039336  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number (n ≥ 1) whose base‑8 representation
   contains the same nonzero number of 1’s and 2’s.  The function
   scans positive integers in increasing order, testing the
   base‑8 digit counts until the nth term is found.  */
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) {  /* offset starts at 1; 0 is undefined */
        return 0;
    }
    seq_t current = 1;           /* start checking from 1 */
    idx_t found = 0;

    while (found < n) {
        seq_t tmp = current;
        idx_t c1 = 0, c2 = 0;

        /* Count digits 1 and 2 in base‑8 representation of current */
        while (tmp > 0) {
            unsigned int digit = (unsigned int)(tmp % 8);
            tmp /= 8;
            if (digit == 1)   c1++;
            else if (digit == 2) c2++;
        }

        if (c1 == c2 && c1 > 0) {
            found++;
            if (found == n) {
                return current;
            }
        }
        current++;
    }

    /* unreachable, but keep compiler happy */
    return current;
}
