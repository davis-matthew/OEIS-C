/*
 * A027781  a(n) = 4*(n+1)*binomial(n+2,8).
 * https://oeis.org/A027781
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (n+1) * binomial(n+2, 8)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = C(n+1, 7) * C(n+2, 2)  (identical, may be used as alternative) */
#include "../utils/gcd.h"

/* Compute binomial coefficient C(N, K) using multiplicative formula,
   reducing by gcd at each step to avoid intermediate overflow. */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t numer = N - K + i;       /* current numerator */
        seq_t denom = i;               /* current denominator */

        seq_t g = gcd(numer, denom);   /* cancel common factor with denom */
        numer /= g;
        denom /= g;

        seq_t g2 = gcd(result, denom); /* cancel common factor with result */
        result /= g2;
        denom /= g2;

        /* denom should now be 1; multiply the remaining factor */
        result *= numer;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 6 in practice, but the formula works for any n. */
    seq_t bin = binom(n + 2, 8);
    bin *= ((seq_t)n + 1);
    bin *= 4;
    return bin;
}
