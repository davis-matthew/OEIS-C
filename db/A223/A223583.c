/*
 * A223583  Number of nX6 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(min(n,6))^(max(n,6)-min(n,6)+1) * ∏_{i=1}^{min(n,6)-1}
 *            U(i)^2 , where U(z) =
 *            (z^6+15*z^5+115*z^4+405*z^3+964*z^2+660*z+720)/720
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precomputed integer values of U(z) for z=1..6. */
    static const seq_t U[7] = {0, 4, 16, 50, 130, 296, 610};

    idx_t minv = (n <= 6) ? n : 6;
    idx_t exp  = (n <= 6) ? (7 - n)  : (n - 5);

    /* Compute the product of U(i)^2 for i = 1 .. minv-1. */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i < minv; ++i)
    {
        seq_t sq = U[i] * U[i];
        prod *= sq;
    }

    /* Compute U(minv)^exp using binary exponentiation to keep
       the number of multiplications moderate. */
    seq_t basePow = (seq_t)1;
    seq_t base = U[minv];
    idx_t e = exp;
    while (e)
    {
        if (e & 1U)
            basePow *= base;
        base *= base;
        e >>= 1U;
    }

    return basePow * prod;
}
