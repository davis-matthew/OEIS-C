/*
 * A129587  a(n) = n!*((1 + 3n + n^2)*H(n) - n), where H(n) is the n-th
 *          harmonic number.
 * https://oeis.org/A129587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n!*((1+3n+n^2)*H(n) - n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the OEIS sequence A129587.
 * n is 1‑based index; for n==0 the function returns 0 (unused). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute factorial and the sum S[n] = sum_{k=1}^n n! / k
     * using the recurrence:
     *   S[1] = 1
     *   For k ≥ 2:
     *       fact_k = k * fact_{k-1}
     *       S[k]   = k * S[k-1] + fact_{k-1}          (since fact_k/k = fact_{k-1})
     */
    seq_t fact = 1;   /* fact_{1} = 1! */
    seq_t sum  = 1;   /* S[1] = 1! / 1 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t fact_k = fact * (seq_t)k;           /* fact_k = k * fact_{k-1} */
        seq_t sum_k  = (seq_t)k * sum + fact;     /* S[k]   = k * S[k-1] + fact_{k-1} */
        fact = fact_k;
        sum  = sum_k;
    }

    /* coeff = 1 + 3n + n^2  (computed with seq_t to avoid overflow of idx_t) */
    seq_t seq_n = (seq_t)n;
    seq_t coeff = seq_n * seq_n + (seq_t)3 * seq_n + (seq_t)1;

    /* a(n) = coeff * sum - n * fact */
    return coeff * sum - seq_n * fact;
}
