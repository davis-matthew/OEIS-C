/*
 * A126709  The Loh-Shu 3 x 3 magic square, variant 2.
 * https://oeis.org/A126709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = [4,9,2,3,5,7,8,1,6][(n-1) mod 9]
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    if (n < 1) return 0;                       /* offset is 1, but guard */
    size_t idx = (size_t)((n - 1) % 9);         /* (n-1) mod 9 */
    return tbl[idx];
}
