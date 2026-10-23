/*
 * A039085  Numbers whose base-9 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑9 representation has equal number of 1's and 3's */

#include <stdbool.h>
#include "../utils/equal13.h"

/* OEIS sequence A039085: numbers whose base‑9 representation has equal
   number of 1’s and 3’s.  a(1) = 0, a(2) = 2, a(3) = 4, ...   */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(0) is undefined but return 0 */
        return 0;

    idx_t pos = 0;       /* how many qualifying numbers we have seen */
    seq_t cand = 0;      /* current candidate number */

    for (;;) {
        if (equal13(cand)) {
            ++pos;
            if (pos == n)
                return cand;
        }
        ++cand;
    }
}
