/*
 * A143125  a(n) = Sum {j=1..n} j*A001462(j).
 * https://oeis.org/A143125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} j * A001462(j) \ * where A001462 is Golomb's
 *            sequence: * g[1] = 1, g[n] = 1 + g[n - g[g[n-1]]] (n≥2). * The
 *            function recomputes the required values on each call in O(n)
 * Range:     u8 n <= 10, u16 n <= 92, u32 n <= 6435, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;          // offset is 1

    /* allocate array for Golomb values; index 0 unused */
    seq_t *g = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!g) return (seq_t)0;                    // allocation failed

    g[(idx_t)1] = (seq_t)1;
    seq_t sum = (seq_t)1 * g[(idx_t)1];         // j=1 term

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* compute g[i] = 1 + g[i - g[g[i-1]]]; */
        idx_t inner = (idx_t)g[(idx_t)g[(idx_t)i-1]];
        idx_t idx   = i - inner;
        g[(idx_t)i] = (seq_t)1 + g[idx];

        /* add i * g[i] to the cumulative sum */
        sum += (seq_t)i * g[(idx_t)i];
    }

    free(g);
    return sum;
}
