/*
 * A071732  Expansion of (1+x^3*C^2)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C3(n) + (n>=3 ? C5(n-3) : 0) * where Cm(k) = m/(2k+m) *
 *            binom(2k+m , k) * and binom(N,K) is the binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: binomial coefficient binom(N,K) for idx_t indices,
   computed multiplicatively to keep intermediate values small. */
static seq_t binom_idx(idx_t N, idx_t K)
{
    if (K > N - K)
        K = N - K;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* res *= (N-K+i) / i   with integer arithmetic */
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

/* Helper: coefficient of x^k in C(x)^m for m=3 or 5.
   Uses C_m(k) = m/(2k+m) * binom(2k+m , k). */
static seq_t coeff(idx_t m, idx_t k)
{
    seq_t bin = binom_idx(2 * k + m, k);
    seq_t num = bin * (seq_t)m;
    seq_t denom = (seq_t)(2 * k + m);
    return num / denom;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Coefficient from C^3. */
    seq_t res = coeff(3, n);

    /* Add contribution from x^3*C^5 for n >= 3. */
    if (n >= 3)
        res += coeff(5, n - 3);

    return res;
}
