/*
 * A228703  Arises in analysis of the halving procedure for detecting two
 *          defectives among n items.
 * https://oeis.org/A228703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 46341 * 2^(n-5) for n >= 5; a(1)=6, a(2)=23, a(3)=91,
 *            a(4)=5793
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)23;
    if (n == 3) return (seq_t)91;
    if (n == 4) return (seq_t)5793;
    /* for n >= 5 */
    return (seq_t)46341 * pow2(n - 5);
}
