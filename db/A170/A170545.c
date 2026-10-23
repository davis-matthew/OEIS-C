/*
 * A170545  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*Σ_{k=1}^{min(n,46)} a(n-k) – 21*a(n-47) + p(n) where
 *            p(0)=1, p(1…46)=2, p(47)=1, p(n)=0 for n≥48. The first term
 *            a(0)=1 and all further terms are produced by this linear
 *            recurrence. The sequence starts 1,8,56,392,2744,… as in A170545
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache of already computed terms. */
static seq_t *a_cache    = NULL;
static idx_t a_cache_size = 0;   /* number of elements stored (next free index) */
static size_t a_cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    /* realloc may return NULL on failure; abort in that case. */
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    a_cache = new_ptr;
    a_cache_cap = new_cap;
}

/* Compute a(n) for any n ≥ 0.  Results are memoised. */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary. */
    ensure_cache(n);

    /* Initialise the cache on first use. */
    if (a_cache_size == 0) {
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_cache_size = 1;
    }

    /* Return cached value if already computed. */
    if (n < a_cache_size) return a_cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* Σ_{k=1}^{min(i,46)} a(i-k) */
        idx_t upper = i < 46 ? i : 46u;
        for (idx_t k = 1; k <= upper; ++k)
            sum += a_cache[i - k];

        seq_t prev47 = (i >= 47) ? a_cache[i - 47] : (seq_t)0;

        seq_t p = 0;
        if (i == 0)           p = 1;        /* p_0 = 1 */
        else if (i <= 46)     p = 2;        /* p_1 … p_46 = 2 */
        else if (i == 47)     p = 1;        /* p_47 = 1 */
        /* for i ≥ 48: p = 0 (already 0) */

        a_cache[i] = 6 * sum - 21 * prev47 + p;
    }

    return a_cache[n];
}
