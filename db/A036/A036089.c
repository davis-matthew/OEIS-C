/*
 * A036089  Centered cube numbers: (n+1)^11 + n^11.
 * https://oeis.org/A036089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^11 + n^11
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 52, u128 n <= 2988,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t n_seq = (seq_t)n;
    /* Compute (n + 1) */
    seq_t n_plus = n_seq + (seq_t)1;

    /* Compute n^11 and (n + 1)^11 using the provided power routine */
    seq_t n_pow  = seq_pow(n_seq, 11);
    seq_t npow   = seq_pow(n_plus, 11);

    /* Sum the results and return */
    return n_pow + npow;
}
