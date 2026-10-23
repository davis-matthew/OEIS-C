/*
 * A013609  Triangle of coefficients in expansion of (1+2*x)^n.
 * https://oeis.org/A013609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   a(n) = T(r,k) where T(r,k)=2^k*binomial(r,k) and * n = r*(r+1)/2
 *            + k, 0≤k≤r. The function flattens the triangle * of coefficients
 *            of (1+2*x)^r by rows. The implementation uses * the supplied
 *            utility functions pow2(idx_t) and binomial(idx_t,idx_t)
 * Range:     u8 n <= 30, u16 n <= 84, u32 n <= 266, u64 n <= 970,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r such that triangular number (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while (1) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t > n) break;
        ++r;
    }

    /* Compute the index offset of row r and the column k */
    unsigned __int128 prev128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t prev = (idx_t)prev128;
    idx_t k = n - prev;

    /* T(r,k) = 2^k * binomial(r,k) */
    seq_t pow = pow2(k);
    seq_t bin = binomial(r, k);
    return pow * bin;
}
