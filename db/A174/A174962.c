/*
 * A174962  a(n) = n^n*(3+n)/2.
 * https://oeis.org/A174962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n*(3+n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute n^n */
    seq_t pow_n = seq_pow(sn, n);

    /* Multiply by (n+3) and divide by 2 */
    seq_t result = (pow_n * (sn + (seq_t)3)) / (seq_t)2;

    return result;
}
