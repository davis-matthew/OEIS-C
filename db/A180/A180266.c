/*
 * A180266  a(0) = 0; a(n) = C(2*n-2,n-1)*(n^2-2*n+2)/n for n >= 1.
 * https://oeis.org/A180266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-2, n-1) * (n^2 - 2*n + 2) / n (a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* m = n-1 ; use it to avoid overflow */
    idx_t m = n - 1;
    idx_t two_m = 2 * m;                /* 2*(n-1) == 2*n-2 */

    /* Compute C(2*n-2, n-1) */
    seq_t bin = binomial(two_m, m);

    /* Since binomial(2*m,m) is divisible by (m+1)=n, divide first */
    seq_t B = bin / (seq_t)n;

    /* Compute (n^2 - 2*n + 2) */
    seq_t nseq = (seq_t)n;
    seq_t factor = nseq * nseq - (seq_t)2 * nseq + (seq_t)2;

    /* Final result */
    return B * factor;
}
