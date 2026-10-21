/*
 * A215772  Number of undirected labeled graphs on n nodes with exactly 2
 *          cycle graphs as connected components.
 * https://oeis.org/A215772
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{k=1}^{n-1} C(n,k) * c(k) * c(n-k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with c(1)=c(2)=1,  c(m)=(m-1)!/2 for m≥3. */

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

/* Cache for c(m) = number of labeled cycle graphs of size m */
static seq_t *c_cache = NULL;
static size_t c_cache_sz = 0; /* allocated size */

/* ensure c_cache holds values up to index n (inclusive) */
static void ensure_c_cache(idx_t n)
{
    if ((idx_t)c_cache_sz > n)
        return;

    size_t new_sz = (size_t)n + 1;   /* indices 0 .. n */
    seq_t *tmp = (seq_t *)realloc(c_cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();
    c_cache = tmp;

    for (idx_t i = c_cache_sz; i <= n; ++i) {
        if (i == 1 || i == 2)
            c_cache[i] = (seq_t)1;
        else if (i == 3)
            c_cache[i] = (seq_t)1;       /* (3-1)!/2 = 1 */
        else
            c_cache[i] = c_cache[i-1] * (seq_t)(i-1); /* (i-1)!/2 */
    }
    c_cache_sz = new_sz;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                  /* values below offset are irrelevant */

    ensure_c_cache(n);

    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;               /* C(n,0) */

    for (idx_t k = 1; k <= n-1; ++k) {
        /* binom = C(n,k) from C(n,k-1) */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / (seq_t)k;

        seq_t term = binom * c_cache[k];
        term = term * c_cache[n - k];
        sum += term;
    }

    return sum / (seq_t)2;                /* divide by two to avoid double counting */
}
