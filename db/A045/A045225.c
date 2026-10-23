/*
 * A045225  Numbers whose base-5 representation contains exactly two 1's and
 *          no 2's.
 * https://oeis.org/A045225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by enumerating consecutive integers and selecting those whose base‑5
   representation contains exactly two digit "1" and no digit "2". */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cand = 0;          /* candidate number */
    idx_t  found = 0;        /* how many valid numbers found so far */

    const seq_t base = 5;    /* base 5 */

    while (found < n) {
        cand++;              /* next integer to test */

        seq_t x = cand;
        int ones = 0;        /* count of digit '1' */

        bool ok = true;      /* whether lucky for digit restrictions */

        /* examine the digits of cand in base 5 */
        while (x > 0) {
            seq_t d = x % base;
            if (d == 2) {    /* forbidden digit '2' */
                ok = false;
                break;
            }
            if (d == 1) {
                ones++;
            }
            x /= base;
        }

        if (!ok)              /* number contains a '2' */
            continue;

        if (ones == 2) {      /* exactly two '1's */
            found++;
        }
    }

    return cand;               /* nth valid number */
}
