/*
 * A103325  Fifth powers of Lucas numbers.
 * https://oeis.org/A103325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n)^5 where L(n) is the n-th Lucas number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    seq_t l = lucas(n);          /* Lucas number L(n) */
    seq_t sq = l * l;            /* L(n)^2 */
    seq_t fourth = sq * sq;      /* L(n)^4 */
    seq_t fifth = fourth * l;    /* L(n)^5 */
    return fifth;
}
