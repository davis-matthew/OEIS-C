/*
 * A160866  512*P_11(n), 512 times the Legendre polynomial of order 13 at n.
 * https://oeis.org/A160866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(n) = Σ_{i=0}^{min(12,n-1)} coeff[i] * binom(n + 12 -
 *            i, 13) (n ≥ 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 10, u128 n <= 328,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Coefficients from the numerator of the generating function
       (without the leading x factor).  */
    static const unsigned long long coeff[13] = {
        512ULL,
        2247605859ULL,
        690419477142ULL,
        25828232616295ULL,
        263754807172680ULL,
        981682771377846ULL,
        1503880076779332ULL,
        981682771377846ULL,
        263754807172680ULL,
        25828232616295ULL,
        690419477142ULL,
        2247605859ULL,
        512ULL
    };

    if (n == 0) return 0;

    /* The number of terms we use is at most 13 but cannot exceed n. */
    idx_t limit = n < 13 ? n : 13;
    seq_t sum = 0;

    for (idx_t i = 0; i < limit; ++i)
    {
        seq_t term = binom(n + 12 - i, (idx_t)13);
        term *= (seq_t)coeff[i];
        sum += term;
    }

    return sum;
}
