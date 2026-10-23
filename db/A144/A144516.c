/*
 * A144516  a(n) = (15*n^2+45*n-70)*binomial(n+4,6)/8.
 * https://oeis.org/A144516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^2+45*n-70)*binomial(n+4,6)/8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t n_seq = n;                     /* n as seq_t */
    seq_t term1 = (seq_t)15 * n_seq * n_seq
                 + (seq_t)45 * n_seq
                 - (seq_t)70;             /* 15n²+45n-70 */

    seq_t bin   = binomial(n + 4, 6);    /* (n+4 choose 6) */

    seq_t res   = term1 * bin;           /* multiply */
    res = res / (seq_t)8;                /* divide by 8 */

    return res;
}
