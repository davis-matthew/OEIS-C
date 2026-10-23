/*
 * A127228  a(n) = numerator of ((n + 4)! - (n - 4)!)/(n!).
 * https://oeis.org/A127228
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n+4)*(n+3)*(n+2)*(n+1)*n*(n-1)*(n-2)*(n-3) - 1
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 255, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* defined for n >= 4, a(4) = 40319 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;           /* undefined for offset; return 0 */
    unsigned __int128 prod = 1;
    idx_t start = n - 3;
    idx_t end   = n + 4;
    for (idx_t i = start; i <= end; ++i) {
        prod *= (unsigned __int128)i;
    }
    return (seq_t)prod - 1;
}
