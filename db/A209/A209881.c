/*
 * A209881  G.f. satisfies: A(x) = 1 + x*[d/dx 1/(1 - x*A(x))].
 * https://oeis.org/A209881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, and for n ≥ 1, a(n) = n * g(n) where g satisfies g(0)
 *            = 1 g(n) = g(n‑1) + ∑_{k=1}^{n‑1} k * g(k) * g(n‑1‑k) (n ≥ 1)
 *            These g(n) are A075834(n). The recurrence follows directly from
 *            the functional equation given in the OEIS entry. The
 *            implementation memoises g up to the largest n requeste...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of g(n) values */
    static seq_t *g = NULL;
    static idx_t  last  = 0;    /* largest n computed so far */
    static size_t cap   = 0;    /* allocated capacity */

    /* initialise cache */
    if (g == NULL) {
        cap = 1;
        g = (seq_t *)malloc(cap * sizeof(seq_t));
        if (g == NULL) { /* allocation failure */
            return (seq_t)0;   /* safe failure value */
        }
        g[0] = (seq_t)1;   /* g(0) = 1 */
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(g, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* realloc failed */
            return (seq_t)0;
        }
        g = tmp;
        cap = new_cap;
    }

    /* compute g values up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t sum = g[m - 1];    /* start with g(m-1) */
        for (idx_t k = 1; k < m; ++k) {
            seq_t term = ((seq_t)k) * g[k];
            term = term * g[m - 1 - k];
            sum += term;
        }
        g[m] = sum;
    }
    last = n;

    /* return a(n) */
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */
    return (seq_t)n * g[n];
}
