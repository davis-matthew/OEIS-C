/*
 * A107508  Coordination sequence for D_13 lattice.
 * https://oeis.org/A107508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{13} c_k · binom(n - k + 12, 12) with c =
 *            [1,299,14664,228800,1557985,5303155,9645688,9645688,
 *            5303155,1557985,228800,14664,299,1]
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Coefficients of (x+1)*(x^12 + 298x^11 + 14366x^10 + 214434x^9
       + 1343551x^8 + 3959604x^7 + 5686084x^6 + 3959604x^5
       + 1343551x^4 + 214434x^3 + 14366x^2 + 298x + 1)          */
    static const uint64_t coeff[14] =
    {
        1ULL,      299ULL,      14664ULL,   228800ULL,   1557985ULL,
        5303155ULL, 9645688ULL, 9645688ULL, 5303155ULL, 1557985ULL,
        228800ULL, 14664ULL,   299ULL,     1ULL
    };

    seq_t sum = 0;
    idx_t maxk = (n < 13) ? n : 13;          /* prevent underflow */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t N = n - k + 12;                 /* n - k ≥ 0 for all k in loop */
        seq_t bin = binom(N, 12);             /* binom(N,12) returns a seq_t   */
        sum += (seq_t)coeff[k] * bin;
    }

    return sum;
}
