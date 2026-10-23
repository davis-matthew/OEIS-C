/*
 * A008376  Coordination sequence for D_9 lattice.
 * https://oeis.org/A008376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n,9)} p[k] * C(n-k+8,8) * where p =
 *            [1,135,2934,18186,43128,43128,18186,2934,135,1]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 221, u128 n <= 1000,
 *            big any n
 * Helpers:   comb_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/comb_8.h"
/*  C(m,8) is computed multiplicatively to avoid overflow.        */

/* Main sequence function                                       */
seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial (1+x)Q(x)       */
    static const unsigned long long p[] = {
        1ULL, 135ULL, 2934ULL, 18186ULL, 43128ULL,
        43128ULL, 18186ULL, 2934ULL, 135ULL, 1ULL
    };
    seq_t total = (seq_t)0;

    /* only sum terms for k <= n                                    */
    idx_t maxk = (n < 9 ? n : 9);
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t N = n - k + 8;                     /* N >= 8               */
        seq_t comb = comb_8(N);
        seq_t term = (seq_t)p[k] * comb;
        total += term;
    }
    return total;
}
