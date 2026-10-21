/*
 * A067296  Fifth column of triangle A028364.
 * https://oeis.org/A067296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067296: a(n) = Sum_{k=0..4} Catalan(k) * Catalan(n+4-k)
/* Catalan(m) = binom(2*m, m) / (m+1)  */
/* Only integer arithmetic is used; all operations are on seq_t */

#include <stdint.h>

/* Compute binomial coefficient C(N, K) using an exact multiplicative method.
   The result is returned as seq_t.  K is reduced to min(K, N-K). */
static seq_t binomial(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* res = res * (N - K + i) / i;   Division is exact */
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

/* Compute Catalan number Cat(m) = binom(2*m, m) / (m+1).  */
static seq_t catalan(idx_t m)
{
    seq_t b = binomial(2 * m, m);
    return b / (seq_t)(m + 1);
}

/* Main function implementing the OEIS formula. */
seq_t a(idx_t n)
{
    /* Catalan numbers for k = 0..4 are constants: 1, 1, 2, 5, 14 */
    const seq_t C0 = 1;
    const seq_t C1 = 1;
    const seq_t C2 = 2;
    const seq_t C3 = 5;
    const seq_t C4 = 14;

    seq_t r0 = catalan(n + 4);  /* Cat(n+4)   */
    seq_t r1 = catalan(n + 3);  /* Cat(n+3)   */
    seq_t r2 = catalan(n + 2);  /* Cat(n+2)   */
    seq_t r3 = catalan(n + 1);  /* Cat(n+1)   */
    seq_t r4 = catalan(n);      /* Cat(n)     */

    seq_t res = r0;
    res += r1;
    res += r2 * C2;
    res += r3 * C3;
    res += r4 * C4;
    return res;
}
