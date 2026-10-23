/*
 * A115005  a(n) = (A114043(n) - 1)/2.
 * https://oeis.org/A115005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(2n-1) + Sum_{i=2..n-1} (n-i)*(2n-i)*phi(i)
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 410, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

/* Computes a(n) for the OEIS sequence A115005. */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0. For n < 1 we simply return 0. */
    if (n <= 1) return 0;

    /* Work in seq_t to avoid overflow of intermediate results. */
    const seq_t N = (seq_t)n;

    /* Baseline term (n-1)*(2n-1) */
    const seq_t twoN = 2 * N;
    seq_t result = (N - 1) * (twoN - 1);

    /* Sum over i = 2 .. n-1 */
    for (idx_t i = 2; i < n; ++i) {
        const seq_t I = (seq_t)i;
        seq_t term = (N - I) * (twoN - I) * phi(i);
        result += term;
    }

    return result;
}
