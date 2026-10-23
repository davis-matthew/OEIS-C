/*
 * A104643  Number of arrangements that can be formed by taking n distinct
 *          things out of 25.
 * https://oeis.org/A104643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = 25!/(25-n)! = product_{k=0}^{n-1} (25 - k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for 0 ≤ n ≤ 25.  For n > 25
       the number of arrangements is zero. */
    if (n > 25)
        return 0;

    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
        res *= (seq_t)(25 - i);
    return res;
}
