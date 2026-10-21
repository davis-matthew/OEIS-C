/*
 * A210000  Number of unimodular 2 X 2 matrices having all terms in
 *          {0,1,...,n}.
 * https://oeis.org/A210000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ( 2*n + 1 + Σ_{d=1}^{n^2} g(d)*g(d+1) ), where g(p) =
 *            #{ (a,d) : 1≤a,d≤n , a*d = p } (ordered pairs)
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate array g of size n*n + 2. Use 16‑bit unsigned integers
       to save memory; each g(p) is at most a few hundred.          */
    size_t maxp = (size_t)n * (size_t)n;          /* n^2 */
    size_t size  = maxp + 2;                      /* indices 0..maxp+1 */
    uint16_t *g = (uint16_t *)calloc(size, sizeof(uint16_t));
    if (g == NULL) return 0;                     /* allocation failure */

    /* Count pairs (a,d) with product p = a*d. */
    for (idx_t a = 1; a <= n; ++a) {
        size_t aa = (size_t)a;
        for (idx_t d = 1; d <= n; ++d) {
            size_t idx = aa * (size_t)d;          /* product ≤ maxp */
            g[idx]++;                            /* ++g[p] */
        }
    }

    /* Compute the sum Σ g(d)*g(d+1). */
    seq_t sum = 0;
    for (size_t d = 1; d <= maxp; ++d) {
        sum += (seq_t)g[d] * (seq_t)g[d + 1];
    }

    free(g);

    seq_t term = (seq_t)(2 * n + 1) + sum;
    return 2 * term;
}
