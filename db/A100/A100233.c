/*
 * A100233  a(n) = Lucas(3*n) - 1.
 * https://oeis.org/A100233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lucas(3*n) - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    idx_t m = n * 3;            /* 3*n fits in idx_t for all n of interest   */
    seq_t val = lucas(m);       /* Lucas number of index 3*n                 */
    val -= (seq_t)1;            /* subtract 1, keeping arbitrary precision  */
    return val;
}
