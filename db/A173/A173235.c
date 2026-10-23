/*
 * A173235  a(n) = n^(n-1) + n - 1.
 * https://oeis.org/A173235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1)+n-1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Treat the index as a sequence element */
    seq_t base = (seq_t)n;
    /* Compute n^(n-1) */
    seq_t power = big_pow(base, base - 1);
    /* Add n-1 */
    return power + base - 1;
}
