/*
 * A126114  Ultimate fixed-point under the mapping n->f(n), where f(n)=n if n
 *          is square else f(n)=n-Floor(Sqrt(n)).
 * https://oeis.org/A126114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = X^2 if X <= k+1 else (X-k-1)^2 where k =
 *            floor((sqrt(1+4*(n-1))-1)/2) and X = n - k*(k+1)
 * Range:     u8 n <= 16, u16 n <= 3215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Check if n is already a perfect square */
    seq_t sq = floor_sqrt((seq_t)n);
    if (sq * sq == (seq_t)n)
        return (seq_t)n;

    /* Compute k = floor((sqrt(1+4*(n-1))-1)/2) */
    seq_t temp = (seq_t)(n - 1);  /* (n-1) fits in idx_t */
    temp = temp * 4 + 1;          /* 1 + 4*(n-1) */
    seq_t root = floor_sqrt(temp);       /* floor sqrt of the above */
    seq_t k = (root - 1) / 2;             /* floor division yields k */

    /* X = n - k*(k+1) */
    seq_t kprod = k * (k + 1);          /* k*(k+1) equals 2*t(k) */
    seq_t X = (seq_t)n - kprod;

    if (X <= k + 1)
        return X * X;
    else {
        seq_t Y = X - k - 1;
        return Y * Y;
    }
}
