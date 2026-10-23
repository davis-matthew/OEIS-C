/*
 * A124497  Number of 1-2-3 trees with n edges and with thinning limbs.
 * https://oeis.org/A124497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n+2} (binomial(3*m,m)*Sum_{k=0..n-m}
 *            ((binomial(2*m+2*k,k)* binomial(n-m-k,2*m+k))/(2*m+k+1))) +
 *            Sum_{k=0..n/2} ((binomial(2*k,k)*binomial(n-k,k))/(k+1))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* sum1 =  Σ_{m=1}^{⌊n/3⌋}  C3m_m * Σ_{k=0}^{n-m}  C(2m+2k,k) * C(n-m-k,2m+k) / (2m+k+1)
       sum2 =  Σ_{k=0}^{⌊n/2⌋}  C(2k,k) * C(n-k,k) / (k+1)    */
    seq_t sum1 = 0;
    seq_t sum2 = 0;

    /* ----- part 1 ----- */
    for (idx_t m = 1; m <= n/3; ++m) {
        seq_t C3m = binomial(3*m, m);
        seq_t A  = 1;                                 /* C(2m+2k, k) */
        seq_t B  = binomial(n - m, 2*m);             /* C(n-m-k, 2m+k) for k=0 */
        idx_t k = 0;
        while (true) {
            /* term = C3m * A * B / (2m+k+1)  using canceling to keep numbers small   */
            seq_t term = C3m;
            seq_t denom = (seq_t)(2*m + k + 1);

            /* cancel with denom step by step */
            seq_t g = seq_gcd(term, denom);
            term /= g;
            denom /= g;

            /* multiply by A */
            term *= A;
            g = seq_gcd(term, denom);
            term /= g;
            denom /= g;

            /* multiply by B */
            term *= B;
            g = seq_gcd(term, denom);
            term /= g;
            denom /= g;   /* denom should now be 1 */

            sum1 += term;

            /* stop if last k */
            if (k == n - m) break;

            /* update A to C(2m+2(k+1), k+1) */
            seq_t num_A = (seq_t)(2*m + 2*k + 2) * (seq_t)(2*m + 2*k + 1);
            seq_t den_A = (seq_t)(k + 1) * (seq_t)(2*m + k + 1);
            A = (A * num_A) / den_A;

            /* update B to C(n-m-(k+1), 2m+(k+1)) */
            seq_t D = (seq_t)(n - 3*m - 2*k);            /* D = n - 3m - 2k ≥ 0 here      */
            seq_t num_B = D * (D - 1);
            seq_t den_B = (seq_t)(n - m - k) * (seq_t)(2*m + k + 1);
            if (D == 0) break;                        /* further B will be zero          */
            B = (B * num_B) / den_B;

            ++k;
        }
    }

    /* ----- part 2 ----- */
    {
        seq_t A2 = 1;        /* C(2k, k)  */
        seq_t B2 = 1;        /* C(n-k, k) */
        idx_t maxk = n / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t term = A2;
            seq_t denom = (seq_t)(k + 1);

            seq_t g = seq_gcd(term, denom);
            term /= g;
            denom /= g;

            term *= B2;
            g = seq_gcd(term, denom);
            term /= g;
            denom /= g;  /* denom should be 1 now */

            sum2 += term;

            if (k == maxk) break;

            /* update A2 to C(2(k+1), k+1) */
            seq_t num_A2 = (seq_t)(2*k + 2) * (seq_t)(2*k + 1);
            seq_t den_A2 = (seq_t)(k + 1) * (seq_t)(k + 1);
            A2 = (A2 * num_A2) / den_A2;

            /* update B2 to C(n-(k+1), k+1) */
            seq_t num_B2 = (seq_t)(n - 2*k) * (seq_t)(n - 2*k - 1);
            seq_t den_B2 = (seq_t)(n - k) * (seq_t)(k + 1);
            B2 = (B2 * num_B2) / den_B2;
        }
    }

    return sum1 + sum2;
}
