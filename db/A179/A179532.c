/*
 * A179532  a(n) = 2^ceiling(n*(n+1)/3).
 * https://oeis.org/A179532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ceil(n*(n+1)/3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n*(n+1) computed in seq_t to avoid overflow of idx_t */
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    /* ceil division by 3: (prod + 2) / 3 */
    seq_t exp = (prod + 2) / 3;
    return pow2((idx_t)exp);
}
