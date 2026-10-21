/*
 * A224678  L.g.f.: -log(1 - Sum_{n>=1} x^(n*(n+1)/2)) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A224678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * G_n - sum_{k=1}^{n-1} G_{n-k} * a(k), where G_m is
 *            the number of compositions of m into positive triangular numbers
 *            (G_0 = 1). This follows from the generating functions L(x) =
 *            -log(1 - Σ x^{k(k+1)/2}) and G(x) = exp(L(x)) = 1/(1 - Σ
 *            x^{k(k+1)/2})
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definitions required by the build environment. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for values already computed. */
    static seq_t *g = NULL, *a_arr = NULL;
    static idx_t computed_up_to = 0;
    static idx_t *tri = NULL;        /* list of triangular numbers ≤ max_n   */
    static size_t tri_cnt = 0;       /* number of triangular numbers stored  */
    static idx_t last_tri_limit = 0; /* highest n for which tri[] was built   */
    static idx_t last_tri_m = 0;     /* largest m with m(m+1)/2 ≤ last_tri_limit */

    if (n == 0) return (seq_t)0;    /* sequence is undefined for n=0 */

    /* Expand cache when needed. */
    if (n > computed_up_to) {
        /* Allocate new space. */
        size_t new_cap = (size_t)n + 1;
        if (g == NULL) {
            g = (seq_t*)malloc(new_cap * sizeof(seq_t));
            a_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
            g[0] = (seq_t)1;
            a_arr[0] = (seq_t)0;
        } else {
            g = (seq_t*)realloc(g, new_cap * sizeof(seq_t));
            a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        }

        /* Extend the list of triangular numbers if necessary. */
        if (n > last_tri_limit) {
            /* Compute triangular numbers up to new n. */
            idx_t m = last_tri_m + 1;
            while (1) {
                idx_t t = m * (m + 1) / 2;
                if (t > n) break;
                tri = (idx_t*)realloc(tri, (tri_cnt + 1) * sizeof(idx_t));
                tri[tri_cnt++] = t;
                m++;
            }
            last_tri_m = m - 1;
            last_tri_limit = n;
        }

        /* Compute G and a for all indices up to n. */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            /* Compute G_i. */
            seq_t gi = 0;
            for (size_t j = 0; j < tri_cnt; ++j) {
                idx_t t = tri[j];
                if (t > i) break;
                gi += g[i - t];
            }
            g[i] = gi;

            /* Compute a_i. */
            seq_t conv = 0;
            for (idx_t k = 1; k < i; ++k) {
                conv += g[i - k] * a_arr[k];
            }
            a_arr[i] = (seq_t)i * gi - conv;
        }

        computed_up_to = n;
    }

    return a_arr[n];
}
