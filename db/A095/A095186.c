/*
 * A095186  a(n) = least k such that k*n^(1/n) differs from an integer by at
 *          most 1/n.
 * https://oeis.org/A095186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that |k * n^(1/n) - nearest integer| ≤ 1/n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /*  n = 1 is trivial */
    if (n == 1)
        return (seq_t)1;

    /* Compute α = n^(1/n) with long double precision */
    const long double inv_n = 1.0L / (long double)n;
    const long double ln_n   = logl((long double)n);
    const long double alpha  = expl(ln_n * inv_n);   /* n^(1/n) = e^(ln n / n) */

    /* Small margin to guard against floating‑point rounding errors */
    const long double eps = 1e-18L;

    /* Search for the smallest k (k ≤ n by Dirichlet’s theorem) */
    for (idx_t k = 1; k <= n; ++k) {
        long double prod = alpha * (long double)k;
        /* Round prod to the nearest integer (half away from zero) */
        long long nearest = (long long)floorl(prod + 0.5L);
        long double diff  = fabsl(prod - (long double)nearest);
        if (diff <= inv_n + eps)
            return (seq_t)k;
    }

    /* The inequality is guaranteed to hold with k = n, so this line
       should never be reached. */
    return (seq_t)n;
}
