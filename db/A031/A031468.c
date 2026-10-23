/*
 * A031468  Numbers whose base-4 representation has 3 fewer 0's than 3's.
 * https://oeis.org/A031468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑4 representation
 *            contains exactly three more ‘3’ digits than ‘0’ digits. (OEIS
 *            A031468)
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 1;          /* candidate integer to test */
    seq_t count = 0;      /* number of terms found so far */

    while (count < n) {
        seq_t t = x;
        unsigned zeros = 0, threes = 0;

        /* count 0’s and 3’s in base‑4 representation of x */
        if (t == 0) {                     /* only when x == 0, which never
                                              is a valid term, but handled
                                              for completeness. */
            zeros = 1;
        } else {
            while (t != 0) {
                unsigned d = (unsigned)(t % 4);
                if (d == 0)      ++zeros;
                else if (d == 3) ++threes;
                t = t / 4;
            }
        }

        if (threes > zeros && (threes - zeros) == 3) {
            ++count;                      /* a valid term found */
            if (count == n) {
                return x;                 /* return the n‑th term */
            }
        }

        ++x;                              /* next integer to test */
    }

    /* This point is never reached for valid n >= 1 */
    return 0;
}
