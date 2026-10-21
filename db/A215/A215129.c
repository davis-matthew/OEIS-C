/*
 * A215129  G.f.: Sum_{n>=0} d^n/dx^n (2*x + x^2)^(2*n) / n!.
 * https://oeis.org/A215129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*g_{n+1} where g(1)=1 and for n≥2 g(n) =
 *            4*∑_{i=1}^{n-1}g(i)g(n-i) + 4*∑_{i=1}^{n-2}g(i)*P(n-i) +
 *            ∑_{i=2}^{n-2}P(i)*P(n-i) with P(k)=∑_{i=1}^{k-1}g(i)g(k-i) (the
 *            recurrence follows from G(x)=x+(2G+G²)² and A(x)=G'(x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* special case: a(0) = 1 */
    if (n == 0u)
        return (seq_t)1u;

    /* we need g[0..n+1] and P[0..n+1] */
    size_t N = (size_t)n + 1u;   /* N = n+1 */

    seq_t *g = (seq_t *)malloc((N + 1u) * sizeof(seq_t));
    seq_t *P = (seq_t *)malloc((N + 1u) * sizeof(seq_t));
    if (!g || !P) {
        /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0u;
    }

    /* initialize base values */
    g[0] = (seq_t)0u;        /* not used, but keep zero for safety */
    g[1] = (seq_t)1u;        /* g1 = 1  (a(0)=1) */
    P[0] = (seq_t)0u;
    P[1] = (seq_t)0u;        /* no pair for degree 1 */

    /* compute g[2..N] iteratively */
    for (size_t i = 2u; i <= N; ++i) {
        /* P[i] = sum_{k=1}^{i-1} g[k] * g[i-k] */
        seq_t sumP = (seq_t)0u;
        for (size_t k = 1u; k < i; ++k)
            sumP += g[k] * g[i - k];
        P[i] = sumP;

        /* Q[i] = sum_{k=1}^{i-2} g[k] * P[i-k] */
        seq_t sumQ = (seq_t)0u;
        for (size_t k = 1u; k + 1u < i; ++k)
            sumQ += g[k] * P[i - k];

        /* R[i] = sum_{k=2}^{i-2} P[k] * P[i-k] */
        seq_t sumR = (seq_t)0u;
        for (size_t k = 2u; k + 2u <= i; ++k)
            sumR += P[k] * P[i - k];

        /* g[i] = 4*P[i] + 4*Q[i] + R[i] */
        g[i] = sumP * (seq_t)4u + sumQ * (seq_t)4u + sumR;
    }

    /* a(n) = (n+1)*g[n+1] */
    seq_t result = g[N] * (seq_t)(n + 1u);

    /* free allocated memory */
    free(g);
    free(P);

    return result;
}
