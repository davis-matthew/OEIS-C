/*
 * A229079  Number A(n,k) of ascending runs in {1,...,k}^n; square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A229079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where i+j is the diagonal s containing n. A(i,j) =
 *            j^(i-1)*((i+1)*j+i-1)/2 for i>0, A(0,j)=0
 * Range:     u8 n <= 38, u16 n <= 72, u32 n <= 180, u64 n <= 451,
 *            u128 n <= 1158, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Determine the diagonal s (s.t. (s+1)*(s+2)/2 > n >= s*(s+1)/2). */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Compute offset within the diagonal. */
    idx_t prev_total = s * (s + 1) / 2;
    idx_t offset    = n - prev_total;
    idx_t i = offset;        /* first index of A (n)   */
    idx_t j = s - offset;    /* second index of A (k)  */

    if (i == 0) {
        return 0;            /* A(0,k) = 0 for all k */
    }

    seq_t k = (seq_t)j;
    seq_t pow_val = big_pow(k, (seq_t)(i - 1));
    seq_t term = ((seq_t)(i + 1) * k) + (seq_t)(i - 1);
    seq_t res = pow_val * term / 2;
    return res;
}
