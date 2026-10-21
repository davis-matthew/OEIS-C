/*
 * A089460  Triangle, read by rows, of coefficients for the second iteration
 *          of the hyperbinomial transform.
 * https://oeis.org/A089460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = floor((sqrt(8*n + 1) - 1)/2) and k = n - r*(r+1)/2. Then
 *            a(n) = T(r,k) where T(r,k) = 2 * (r-k+2)^(r-k-1) * C(r,k) (with
 *            T(r,r) = 1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 152, u128 n <= 405,
 *            big any n
 * Helpers:   binomial, floor_isqrt_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute row index r */
    idx_t tmp = 8ULL * n + 1ULL;
    idx_t r   = (floor_isqrt_u64(tmp) - 1) / 2;

    /* Compute column index k */
    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;

    /* Diagonal element */
    if (k == r)
        return (seq_t)1;

    /* Distance from the diagonal */
    idx_t d   = r - k;
    seq_t base    = (seq_t)(d + 2);
    int   exp     = (int)(d - 1);                 /* d >= 1 here */
    seq_t power   = pow_seq(base, exp);
    seq_t binom   = binomial(r, k);

    /* Combine */
    seq_t result = binom;
    result *= (seq_t)2;
    result *= power;
    return result;
}
