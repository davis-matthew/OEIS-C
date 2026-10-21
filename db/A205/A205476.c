/*
 * A205476  G.f.: exp( Sum_{n>=1} (x^n/n) * Product_{d|n} (1 + n*x^d/d) ).
 * https://oeis.org/A205476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205476: a(n) = coefficient of x^n in
   exp( Σ_{m≥1} (x^m/m) * Π_{d|m} (1 + m*x^d/d) )      */

/* NOTE: The algorithm computes integer values h_k = k * g_k where
   g_k are the coefficients of the logarithm series.  Then
   a(n) satisfies   n*a(n) = Σ_{k=1}^n h_k * a(n-k).
   All arithmetic is performed in type seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Public function as required by the signature */
seq_t a(idx_t n)
{
    /* Handle the offset n=0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Arrays for h, a and temporary dp */
    seq_t *h  = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *a  = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));

    if (!h || !a || !dp) return (seq_t)0; /* allocation failure */

    a[0] = (seq_t)1;          /* a(0) = 1 */

    /* ---------------------------------------------------------------- */
    /* 1.  Compute h_k for k = 1 .. n */
    /* ---------------------------------------------------------------- */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t L = n - m;                 /* maximum subset sum needed */

        /* Initialise dp[0] = 1, dp[1..L] = 0 */
        dp[0] = (seq_t)1;
        for (idx_t t = 1; t <= L; ++t) dp[t] = (seq_t)0;

        /* Enumerate divisors of m and update the subset‑sum dp */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d != 0) continue;

            /* First divisor: d */
            seq_t w_d = (seq_t)(m / d);
            for (long long t = (long long)L; t >= (long long)d; --t) {
                dp[(idx_t)t] = dp[(idx_t)t] + dp[(idx_t)(t - d)] * w_d;
            }

            /* Paired divisor m/d, if distinct */
            idx_t d2 = m / d;
            if (d2 != d) {
                seq_t w_d2 = (seq_t)(m / d2);   /* equal to d */
                for (long long t = (long long)L; t >= (long long)d2; --t) {
                    dp[(idx_t)t] = dp[(idx_t)t] + dp[(idx_t)(t - d2)] * w_d2;
                }
            }
        }

        /* Accumulate contributions to h_k */
        for (long long t = 0; t <= (long long)L; ++t) {
            idx_t k = m + (idx_t)t;              /* 1 ≤ k ≤ n */
            seq_t temp = dp[(idx_t)t] * (seq_t)k; /* dp[t] * k  */
            seq_t add   = temp / (seq_t)m;        /* exact division */
            h[k] = h[k] + add;
        }
    }

    /* ---------------------------------------------------------------- */
    /* 2.  Compute a(n) using n*a(n) = Σ_{k=1}^n h_k * a(n-k) */
    /* ---------------------------------------------------------------- */
    for (idx_t nn = 1; nn <= n; ++nn) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= nn; ++k) {
            sum = sum + h[k] * a[nn - k];
        }
        a[nn] = sum / (seq_t)nn;   /* division is exact */
    }

    /* ---------------------------------------------------------------- */
    seq_t result = a[n];
    /* free temporary arrays */
    free(h);
    free(a);
    free(dp);
    return result;
}
