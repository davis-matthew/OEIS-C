/*
 * A039567  Numbers whose base-5 representation has the same number of 0's,
 *          2's and 4's.
 * https://oeis.org/A039567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 4486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _eq012c4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A039567  */
/*  Numbers whose base‑5 representation has the same number of 0's, 2's and 4's. */
/*  The routine scans integers starting at 1, counting terms until the n‑th
    term is found.  This is O(n) with modest constant factors and works for
    all supported seq_t types (unsigned integers or an arbitrary‑precision
    integer class).  */
#include <stdbool.h>
#include "../utils/_eq012c4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n==0, return 0 safely */

    idx_t count = 0;
    seq_t val   = 1;

    while (true) {
        if (_eq012c4(val)) {
            count++;
            if (count == n)
                return val;
        }
        val++;
    }
}
