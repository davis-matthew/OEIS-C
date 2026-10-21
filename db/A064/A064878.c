/*
 * A064878  Generalized Catalan numbers C(10,10; n).
 * https://oeis.org/A064878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; for n >= 2 let k = n-2. a(n) = 10^(k+1)/(k+1) *
 *            Σ_{m=0}^{k} (m+1)(m+2) * binomial(2k-m, k-m) * 10^(k-m)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n < 2)
        return (seq_t)1;

    idx_t k = n - 2;                /* k = n-2,   k >= 0   */
    seq_t B = binomial(2 * k, k);   /* B_0 = binomial(2k, k) */
    seq_t power10 = pow10(k);        /* 10^k              */
    seq_t S = 0;                     /* Accumulator for the sum */

    for (idx_t m = 0; m <= k; ++m) {
        seq_t mult   = (seq_t)(m + 1) * (seq_t)(m + 2);
        seq_t term   = mult * B * power10;
        S += term;

        if (m < k) {
            /* B_{m+1} = B_m * (k-m) / (2k-m)  */
            seq_t numer = (seq_t)(k - m);
            seq_t denom = (seq_t)(2 * k - m);
            B = (B * numer) / denom;

            /* 10^(k-(m+1)) = 10^(k-m) / 10   */
            power10 = power10 / 10;
        }
    }

    seq_t bigPow     = pow10(k + 1);          /* 10^(k+1)                 */
    seq_t numerator  = bigPow * S;            /* 10^(k+1) * S             */
    seq_t denominator = (seq_t)(k + 1);       /* (k+1) = (n-1)             */
    return numerator / denominator;
}
