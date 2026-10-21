/*
 * A091039  Triangle of scaled second column sequences of (k,k)-Stirling2
 *          arrays.
 * https://oeis.org/A091039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n, k) = (k!^(n-k+1))*((k+1)^(n-k+1)-1)/(k*k!) for n>=k>=1. The
 *            sequence a(n) is obtained by reading the triangular array
 *            row‑wise: a(n) = a(r,c) where r = ceil((sqrt(8*n+1)-1)/2), c = n
 *            - (r-1)*r/2
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 39, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0)  /* offset is 1; undefined for n=0 */
        return 0;

    /* Locate the position (r,c) in the triangle. */
    idx_t r = (idx_t)((sqrt((long double)1 + 8 * (long double)n) - 1) / 2);
    if (r * (r + 1) / 2 < n) r++;          /* adjust upward if needed */
    while ((r - 1) * r / 2 >= n) r--;      /* adjust downward if overshoot */
    idx_t c = n - (r - 1) * r / 2;          /* 1‑based column index */
    idx_t e = r - c + 1;                   /* power exponent */

    /* Compute factorial of c (small, so do it inline). */
    seq_t F = (seq_t)1;
    for (idx_t i = 2; i <= c; ++i)
        F *= (seq_t)i;

    /* F^(e-1) */
    seq_t F_pow = big_pow(F, (seq_t)(e - 1));
    /* (c+1)^e - 1 */
    seq_t G = big_pow((seq_t)(c + 1), (seq_t)e);
    G -= (seq_t)1;

    /* Final result = F_pow * G / c */
    seq_t num = F_pow * G;
    seq_t res = num / (seq_t)c;

    return res;
}
