/*
 * A017277  a(n) = (10*n)^9.
 * https://oeis.org/A017277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 500,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute n^9 */
    seq_t n9 = seq_pow((seq_t)n, 9);
    /* multiply by 10^9 */
    return n9 * pow10(9);
}
