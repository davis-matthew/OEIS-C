/*
 * A000272  Number of trees on n labeled nodes: n^(n-2) with a(0)=1.
 * https://oeis.org/A000272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, core, nice, changed
 * Formula:   a(n) = n^(n-2) for n >= 2, a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;
    return a000272_ipow((seq_t)n, n - 2);
}
