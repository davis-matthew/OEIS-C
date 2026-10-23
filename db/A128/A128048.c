/*
 * A128048  a(n) = (n^3+n)*8^n.
 * https://oeis.org/A128048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3+n)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* use seq_t arithmetic for the index */
    seq_t ns = (seq_t)n;

    /* n^3 + n = n * (n^2 + 1) */
    seq_t n_sq = ns * ns;                 /* n^2   */
    seq_t n_cub_plus = ns * (n_sq + (seq_t)1);  /* n^3 + n */

    /* 8^n using the generic integer power routine */
    seq_t pow8 = pow_int((seq_t)8, n);

    /* final result */
    return n_cub_plus * pow8;
}
