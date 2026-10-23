/*
 * A012393  Expansion of e.g.f. arctanh(tan(x)*tan(x)) (even powers only).
 * https://oeis.org/A012393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A012393
 * Formula: a(0)=0, a(n)=2*∑_{i=1}^{n-1} a(i)*a(n-i)*C(2n-2,2i-1)   (n≥1)
 * with C(n,k) binomial coefficient.  */
#include <stdlib.h>

/* === Helper: binomial coefficient ------------------------------------ */
static seq_t binom(idx_t n, idx_t k)
{
    /* use symmetry to minimise loops */
    if (k > n - k)
        k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;              /* exact division */
    }
    return result;
}

/* === Main sequence computation -------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache     = NULL;   /* array of computed a(i) */
    static size_t  capacity = 0;      /* allocated size of cache    */
    static idx_t   last_n   = 0;      /* highest n for which cache is filled */

    /* a(0) is trivial */
    if (n == 0u)
        return (seq_t)0u;

    /* Ensure cache large enough */
    if (cache == NULL) {                          /* first call */
        capacity = (size_t)n + 1;
        if (capacity < 2u)
            capacity = 2u;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)                 /* allocation failure – abort */
            return (seq_t)0u;
        cache[0] = (seq_t)0u;
        if (capacity > 1u)
            cache[1] = (seq_t)2u;
        last_n = 1u;
    } else if ((size_t)n + 1 > capacity) {        /* grow cache if needed */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                               /* allocation failed */
            free(cache);
            cache = NULL;
            capacity = 0;
            return (seq_t)0u;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Return already computed value */
    if (n <= last_n)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        if (k == 1u) {          /* a(1) already set to 2 */
            cache[1u] = (seq_t)2u;
            continue;
        }

        seq_t sum = (seq_t)0u;
        for (idx_t i = 1u; i < k; ++i) {
            seq_t term = binom(2u * k - 2u, 2u * i - 1u);
            term *= cache[(size_t)i];
            term *= cache[(size_t)(k - i)];
            sum += term;
        }
        cache[(size_t)k] = sum * (seq_t)2u;
    }

    last_n = n;
    return cache[(size_t)n];
}
