/*
 * A129008  a(n) = (n^3 + n^2)*8^n.
 * https://oeis.org/A129008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2) * 8^n (offset 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Promote index to seq_t to preserve precision before arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute n^2 and n^3 */
    seq_t n_sq  = n_seq * n_seq;      /* n^2     */
    seq_t n_cub = n_sq * n_seq;       /* n^3     */

    /* (n^3 + n^2) */
    seq_t coeff = n_cub + n_sq;

    /* 8^n */
    seq_t pow8 = seq_pow((seq_t)8, n);

    /* Result */
    return coeff * pow8;
}
