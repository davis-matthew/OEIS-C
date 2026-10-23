/*
 * A178920  E.g.f. A(x) satisfies A(x) = x * (1 + A(x)) * exp( A(x) * (1 +
 *          A(x)) ).
 * https://oeis.org/A178920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{i=0..n-1} binomial(n+i, n-i-1) * n^i / i!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A178920.
 * The function is memoized only in the sense that it does not
 * rely on global state; it recalculates the value efficiently
 * for each call independent of previous calls. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Below the offset the sequence is undefined; return 0. */
    if (n == 0) return 0;

    /* Compute (n-1)! */
    seq_t fact_nm1 = 1;
    for (idx_t k = 2; k <= n - 1; ++k)
        fact_nm1 *= (seq_t)k;

    /* Initial binomial value: binom(n, n-1) == n */
    seq_t B = (seq_t)n;
    /* n^i, starting with i = 0 */
    seq_t power_n = 1;
    /* (n-1)! / i! for current i, starting with i = 0 */
    seq_t fac_ratio = fact_nm1;

    seq_t sum = 0;

    for (idx_t i = 0; i < n; ++i) {
        /* term = B * n^i * (n-1)!/i! */
        seq_t term = B;
        term *= power_n;
        term *= fac_ratio;
        sum += term;

        if (i == n - 1) break;   /* No update needed after last term */

        /* Update (n-1)!/i! for the next i: divide by (i+1) */
        fac_ratio /= (i + 1);

        /* Update n^i for the next i: multiply by n */
        power_n *= (seq_t)n;

        /* Update binomial B to B_{i+1}:
           B_{i+1} = B_i * (n+i+1) * (n-i-1) / ((2*i+3)*(2*i+2)) */
        idx_t denom_idx = (2 * i + 3) * (2 * i + 2);
        seq_t denom = (seq_t)denom_idx;

        seq_t num = B;
        num *= (seq_t)(n + i + 1);
        num *= (seq_t)(n - i - 1);

        B = num / denom;
    }

    return sum;
}
