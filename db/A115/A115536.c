/*
 * A115536  Numbers k such that the square of k is the concatenation of two
 *          numbers m and 4*m.
 * https://oeis.org/A115536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 160378 + 9434 * (n-1)
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n <= 0, return 0 as sentinel */

    /* Convert index to seq_t before performing arithmetic */
    seq_t nm1 = (seq_t)(n - 1);
    seq_t base = (seq_t)160378ULL;
    seq_t diff = (seq_t)9434ULL;

    return base + diff * nm1;
}
