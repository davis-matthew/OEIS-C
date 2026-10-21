/*
 * A067324  Third column of triangle A067323.
 * https://oeis.org/A067324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3) - (C(n+2) + C(n+1)), where C(k) is the k-th
 *            Catalan number
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c_n1 = catalan(n + 1);     /* C(n+1) */
    seq_t c_n2 = catalan(n + 2);     /* C(n+2) */
    seq_t c_n3 = catalan(n + 3);     /* C(n+3) */
    return c_n3 - (c_n2 + c_n1);
}
