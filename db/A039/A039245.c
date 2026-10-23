/*
 * A039245  Numbers whose base-12 representation has the same number of 3's
 *          and 10's.
 * https://oeis.org/A039245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * A039245: numbers whose base-12 representation has the same
 *            number of 3's and 10's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* offset 1: a(1)=0 */
    idx_t found = 0;
    seq_t value = 0;

    while (found < n) {
        /* Count digits 3 and 10 in base‑12 representation of value */
        seq_t tmp = value;
        unsigned count3 = 0, count10 = 0;

        /* For value == 0 the loop below is skipped, counts stay 0 */
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % (seq_t)12);
            if (digit == 3)
                ++count3;
            else if (digit == 10)
                ++count10;
            tmp /= (seq_t)12;
        }

        if (count3 == count10) {
            ++found;
            if (found == n)
                return value;          /* nth term found */
        }

        ++value;                       /* next integer to test */
    }

    /* unreachable, but keep compiler happy */
    return value;
}
