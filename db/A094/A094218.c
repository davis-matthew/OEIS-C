/*
 * A094218  Number of permutations of length n with exactly 2 occurrences of
 *          the pattern 2-13.
 * https://oeis.org/A094218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * binomial(2*n, n-4) / 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(N,K) using a multiplicative
   algorithm with reduction by gcd to avoid intermediate overflow.
   The routine assumes seq_t supports +, -, *, /, %, +, <, >>, <<,
   and that there is a function seq_t gcd(seq_t, seq_t) available. */
#include <stdint.h>
#include "../utils/gcd.h"

/* Helper: compute C(N,K) with K <= N */
static seq_t binomial_reduced(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0 || K == N) return (seq_t)1;
    /* use symmetry */
    if (K > N - K) K = N - K;

    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i); /* numerator term */
        seq_t den = (seq_t)i;          /* denominator term */

        /* reduce num/den by gcd */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* reduce r with remaining denominator part */
        g = gcd(r, den);
        r /= g;
        den /= g;  /* den now is 1 */

        /* multiply */
        r *= num;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 4ULL) return (seq_t)0;

    /* binom(2*n, n-4) */
    idx_t two_n = n << 1;
    idx_t k = n - 4;
    seq_t binom = binomial_reduced(two_n, k);

    /* compute n * binom / 2 avoiding overflow when n is even */
    if ((n & 1ULL) == 0ULL)
    {
        return binom * (seq_t)(n >> 1);
    }
    else
    {
        return (binom / 2) * (seq_t)n;
    }
}
