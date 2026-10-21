/*
 * A212948  Number of 6Xn 0..n-1 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (-1)^n * h[n] * where h[0] = 1 and for m ≥ 1 * h[m] =
 *            -∑_{k=1}^{m} C(n + k – 1, k)^6 * h[m–k] * * C(n + k – 1, k) =
 *            binomial coefficient “(n+k-1 choose k)”. * The power 6 is taken
 *            in the type seq_t; all arithmetic uses * the operators supplied
 *            for seq_t ( + – * / % )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* use a size_t for allocating arrays */
    size_t N = (size_t)n;
    if (N == 0u) return 0;

    /* allocate arrays for h, g (binomial), and f (=g^6) */
    seq_t *h = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *g = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *f = (seq_t*)malloc((N + 1) * sizeof(seq_t));

    /* base values */
    h[0] = (seq_t)1;
    g[0] = (seq_t)1;
    f[0] = (seq_t)1;   /* unused, harmless */

    /* Precompute binomial coefficients g[k] = C(n + k – 1, k) */
    for (size_t k = 1; k <= N; ++k) {
        seq_t numerator = g[k - 1] * (seq_t)(n + k - 1);
        g[k] = numerator / (seq_t)k;
    }

    /* f[k] = g[k]^6 */
    for (size_t k = 1; k <= N; ++k) {
        seq_t t = g[k];
        seq_t t2 = t * t;          /* t^2 */
        seq_t t4 = t2 * t2;        /* t^4 */
        seq_t t6 = t4 * t2;        /* t^6 */
        f[k] = t6;
    }

    /* Dynamic programming for h[n] */
    for (size_t m = 1; m <= N; ++m) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= m; ++k) {
            sum += f[k] * h[m - k];
        }
        h[m] = -sum;    /* h[m] = -sum */
    }

    /* Result: a(n) = (-1)^n * h[n] */
    seq_t result = (n & 1) ? -h[N] : h[N];

    /* free allocated memory */
    free(h);
    free(g);
    free(f);

    return result;
}
