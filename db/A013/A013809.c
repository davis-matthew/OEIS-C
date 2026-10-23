/*
 * A013809  a(n) = 18^(4*n + 3).
 * https://oeis.org/A013809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent 4*n + 3 as seq_t */
    seq_t exp = (seq_t)(4 * n + 3);
    /* Evaluate 18^exp using the generic big_pow routine */
    return big_pow((seq_t)18, exp);
}
