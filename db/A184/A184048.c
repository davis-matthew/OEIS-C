/*
 * A184048  T(n,k) = 1/9 the number of (n+1) X (k+1) 0..2 arrays with all 2 X
 *          2 subblocks having the same four values.
 * https://oeis.org/A184048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where i = n - (s-1)(s-2)/2, k = s - i, T(i,k) = -7
 *            + 4*(2^(i-1)+2^(k-1)) + 2*(2^(floor((i-1)/2))+2^(floor(i/2)) +
 *            2^(floor((k-1)/2))+2^(floor(k/2)))
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 391, u64 n <= 391,
 *            u128 n <= 391, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence lists values of T(n,k) for all 1≤n,k,
    enumerated by increasing sum s = n+k, and within each
    anti–diagonal by increasing n.  This function computes
    a(n) directly from the explicit formula. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal (sum) containing the n‑th term. */
    idx_t s = 1;
    while (s * (s - 1) / 2 < n) {
        ++s;
    }

    /* Number of terms before diagonal s */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* Offset of the desired term within the diagonal (0‑based). */
    idx_t offset = n - prev - 1;

    /* Corresponding indices (i, k).  i runs from 1 to s‑1. */
    idx_t i = offset + 1;
    idx_t k = s - i;

    /* Compute the expression.  All arithmetic is done in seq_t. */
    seq_t pow_i_1   = pow2(i - 1);
    seq_t pow_k_1   = pow2(k - 1);
    seq_t pow_fi_1  = pow2((i - 1) / 2);
    seq_t pow_fi_2  = pow2(i / 2);
    seq_t pow_fk_1  = pow2((k - 1) / 2);
    seq_t pow_fk_2  = pow2(k / 2);

    seq_t value = 4 * (pow_i_1 + pow_k_1);                                       /* 4*(...)   */
    value += 2 * (pow_fi_1 + pow_fi_2 + pow_fk_1 + pow_fk_2);                   /* 2*(...)   */
    value -= 7;                                                                   /* -7        */

    return value;
}
