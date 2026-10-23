/*
 * A035605  Number of points of L1 norm 11 in cubic lattice Z^n.
 * https://oeis.org/A035605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{min(n,11)} binom(n,i) * binom(n+10-i, 11-i)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small binomial coefficient: binom(N,K) with 0 <= K <= N, N up to 2^64-1 */
static inline seq_t binom_small(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0 || K == N) return (seq_t)1;
    if (K > N - K) K = N - K;          /* use symmetry to reduce loop size */
    seq_t res = (seq_t)1;
    for (idx_t j = 1; j <= K; ++j) {
        res = res * (N - K + j);     /* multiply by next numerator term */
        res = res / j;               /* divide by current denominator term */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t max_i = (n < 11) ? n : 11;    /* upper limit of summation */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t c1 = binom_small(n, i);                      /* binom(n,i) */
        seq_t c2 = binom_small(n + 10 - i, 11 - i);         /* binom(n+10-i, 11-i) */
        result += c1 * c2;
    }
    return result;
}
