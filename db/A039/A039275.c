/*
 * A039275  Numbers whose base-3 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   balanced_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039275: numbers whose base‑3 representation has the same non‑zero number
   of 0's and 1's.  This implementation searches integers in increasing
   order and returns the nth such integer (1‑based index). */
#include <stdbool.h>
#include "../utils/balanced_base3.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n<1, return 0 */

    idx_t count = 0;
    for (idx_t v = 1; ; ++v)
    {
        if (balanced_base3(v))
        {
            ++count;
            if (count == n)
                return (seq_t)v;
        }
    }
}
