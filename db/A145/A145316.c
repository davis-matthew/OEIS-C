/*
 * A145316  L-matrix for Hankel matrix of aerated factorials.
 * https://oeis.org/A145316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8*n + 1) - 1)/2), k = n -
 *            r*(r + 1)/2, and T(r, k) = ((r-k)/2)! * C( (r+k)/2 - ⌊k/2⌋ ,
 *            ⌊(k+1)/2⌋ ) * C( (r+k)/2 - ⌊(k+1)/2⌋ , ⌊k/2⌋ ) * (1 +
 *            (-1)^(r-k))/2. This implements the flattened triangle from OEIS
 *            entry A145316
 * Range:     u8 n <= 56, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute row r and column k corresponding to the flattened index n */
    long double d = sqrtl((long double)8 * (long double)n + 1);
    idx_t r = (idx_t)((d - 1) / 2);

    /* Correct possible rounding errors */
    while (((unsigned __int128)r * (r + 1) / 2) > n)
        --r;
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t offset = r * (r + 1) / 2;   /* total entries before row r */
    idx_t k = n - offset;             /* column within row */

    /* If r and k have different parity, the term is zero */
    if ((r & 1) != (k & 1))
        return (seq_t)0;

    /* Compute j = (r - k) / 2 */
    idx_t j = (r - k) / 2;

    if ((r & 1) == 0) /* r is even: r = 2p */
    {
        idx_t p = r / 2;
        seq_t fact = factorial(j);
        seq_t c   = binomial(p, j);
        return fact * c * c;
    }
    else /* r is odd: r = 2p + 1 */
    {
        idx_t p = (r - 1) / 2;
        seq_t fact = factorial(j);
        seq_t c1   = binomial(p + 1, j);
        seq_t c2   = binomial(p, j);
        return fact * c1 * c2;
    }
}
