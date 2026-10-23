/*
 * A192830  Molecular topological indices of the halved cube graphs.
 * https://oeis.org/A192830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3)*n^2*(n-1)*(n-1+2^(n-2)) (n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* cast the index to seq_t for arithmetic */
    seq_t sn        = (seq_t)n;
    seq_t sn_minus1 = sn - (seq_t)1;

    /* 2^(n-3) and 2^(n-2) */
    seq_t two_pow_n_minus3 = pow2(n - 3);
    seq_t two_pow_n_minus2 = pow2(n - 2);

    /* compute the product */
    seq_t term = two_pow_n_minus3 * sn * sn * sn_minus1 *
                 (sn_minus1 + two_pow_n_minus2);

    return term;
}
