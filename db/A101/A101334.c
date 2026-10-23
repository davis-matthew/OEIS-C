/*
 * A101334  a(n) = n^n - (n+1)^(n-1).
 * https://oeis.org/A101334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - (n+1)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 as defined by the OEIS entry */
    if (n == 0) return (seq_t)0;

    /* Compute n^n */
    seq_t term1 = pow_seq((seq_t)n, n);

    /* Compute (n+1)^(n-1) */
    seq_t term2 = pow_seq((seq_t)(n + (seq_t)1), n - (idx_t)1);

    return term1 - term2;
}
