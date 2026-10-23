/*
 * A032035  Number of increasing rooted 2,3 cacti (triangular cacti with
 *          bridges) with n-1 nodes.
 * https://oeis.org/A032035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(1)=a(2)=1; a(n)=∑_{k=1}^{n-2} binomial(n-2,k)*a(k)*a(n-k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive computation with memoisation.  The routine can be called
 * in any order.  All intermediate values are computed using the
 * seq_t arithmetic operators, so the implementation works for both
 * fixed-width and arbitrary-precision builds. */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* for uintptr_t if needed */

static seq_t *cache     = NULL;   /* array of computed a(n) */
static idx_t  cache_lim = 0;      /* size of *cache array (index 0..n) */
static idx_t  computed  = 0;      /* last index that has been computed */

/* Ensure that cache has space for indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_lim) return;                /* already large enough */

    size_t new_size = (size_t)n + 1;           /* +1 because we use 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) { /* OOM – terminate (malloc behaviour) */ }
    /* Initialise the newly added elements to zero.  For seq_t this is
     * equivalent to seq_t zero = 0. */
    for (idx_t i = cache_lim; i <= n; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_lim = new_size;
}

/* The public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute any missing terms up to n.  The recurrence only needs
     * values with indices < n, so we can compute in increasing order. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = (seq_t)1;
        } else {
            idx_t m = i - 2;          /* upper limit for k */
            seq_t sum = 0;            /* running total */

            /* cur holds binomial(m,k).  Start with k = 1 : binomial(m,1)=m. */
            seq_t cur = (seq_t)m;

            for (idx_t k = 1; k <= m; ++k) {
                /* term = cur * cache[k] * cache[i-k] */
                seq_t term = cache[k] * cache[i - k];
                term = term * cur;
                sum += term;

                /* Update cur to binomial(m,k+1) if k < m. */
                if (k < m) {
                    /* cur = cur * (m - k) / (k + 1)  (exact division) */
                    seq_t num = cur * (seq_t)(m - k);
                    cur = num / (seq_t)(k + 1);
                }
            }
            cache[i] = sum;
        }
    }
    computed = n;
    return cache[n];
}
