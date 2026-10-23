/*
 * A192246  0-sequence of reduction of tetrahedral number sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 *            x*(1-4x+12x^2-12x^3+5x^4)/(1-x)/(1-x-x^2)^4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Function computing A192246. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate working arrays. */
    seq_t *g = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    seq_t *h = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!g || !h) {
        /* Allocation failed – return 0. */
        if (g) free(g);
        if (h) free(h);
        return 0;
    }

    /* Base value from 1/(1-x-x^2)^4. */
    g[0] = 1;

    /* Compute g[n] using the recurrence
       g[n] = 4g[n-1] - 2g[n-2] - 8g[n-3] + 5g[n-4]
              + 8g[n-5] - 2g[n-6] - 4g[n-7] - g[n-8]. */
    for (idx_t i = 1; i < n; ++i) {
        seq_t val = 0;
        if (i >= 1) val += (seq_t)4 * g[i - 1];
        if (i >= 2) val -= (seq_t)2 * g[i - 2];
        if (i >= 3) val -= (seq_t)8 * g[i - 3];
        if (i >= 4) val += (seq_t)5 * g[i - 4];
        if (i >= 5) val += (seq_t)8 * g[i - 5];
        if (i >= 6) val -= (seq_t)2 * g[i - 6];
        if (i >= 7) val -= (seq_t)4 * g[i - 7];
        if (i >= 8) val -= (seq_t)1 * g[i - 8];
        g[i] = val;
    }

    /* Compute cumulative sums h[n] = Σ_{k=0}^{n} g[k]. */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += g[i];
        h[i] = sum;
    }

    /* Evaluate a(n) = h[n-1] - 4h[n-2] + 12h[n-3] - 12h[n-4] + 5h[n-5]. */
    seq_t result = 0;
    if (n >= 1) result += h[n - 1];
    if (n >= 2) result -= (seq_t)4 * h[n - 2];
    if (n >= 3) result += (seq_t)12 * h[n - 3];
    if (n >= 4) result -= (seq_t)12 * h[n - 4];
    if (n >= 5) result += (seq_t)5 * h[n - 5];

    /* Clean up. */
    free(g);
    free(h);

    return result;
}
