/*
 * A135965  a(n) = binomial(floor(n*(1+sqrt(2))),n) for n>=0.
 * https://oeis.org/A135965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(floor(n*(1+sqrt(2))), n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

/* Compute binomial coefficient C(N,k) for 0 ≤ k ≤ N, where
 * N is a seq_t and k is an idx_t.  The calculation is performed in a
 * multiplicative way with reductions via gcd to avoid overflow.  It
 * works for arbitrary‑precision seq_t or for 64‑bit values. */
static seq_t binom_reduced(seq_t N, idx_t K)
{
    /* Shortcut cases */
    if (K == 0 || K == (idx_t)N)
        return (seq_t)1;

    /* Use the symmetry property C(N,K) = C(N,N-K) */
    if (K > (idx_t)N - K)
        K = (idx_t)N - K;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num    = (seq_t)(N - (seq_t)K + (seq_t)i); /* N-K+i */
        seq_t denom  = (seq_t)i;

        seq_t g = gcd(num, denom);      /* reduce numerator/denominator */
        num   /= g;
        denom /= g;

        g = gcd(result, denom);        /* reduce result/denominator */
        result /= g;
        denom /= g;                    /* denom should now be 1 */

        result *= num;                 /* safe multiplication now */
    }
    return result;
}

/* OEIS A135965: a(n) = binomial(floor(n*(1+sqrt(2))), n) for n ≥ 0 */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;               /* promote n to seq_t */

    /* floor(n*sqrt(2)) = floor_sqrt(2*n*n) */
    seq_t two_n_sq = (seq_t)2 * n_seq * n_seq;
    seq_t k = floor_sqrt(two_n_sq);       /* floor(n*sqrt(2)) */

    seq_t N = n_seq + k;                  /* floor(n*(1+sqrt(2))) */

    return binom_reduced(N, n);
}
