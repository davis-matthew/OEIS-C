/*
 * A211367  Array read by antidiagonals: T(m,n) = m * Sum(1<=i<=m) (m+n-2+i)!
 * https://oeis.org/A211367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value of the array T(m,k) read by antidiagonals, *
 *            where T(m,k) = m * Σ_{i=1..m} (m+k-2+i)! . The indices (m,k) are
 *            * obtained from the n-th entry in antidiagonal order: first
 *            (1,1), * then (1,2),(2,1), then (1,3),(2,2),(3,1), etc
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 64, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;    /* factorial cache: fact_cache[i] = i! */
static idx_t  fact_len   = 0;       /* number of cached factorials   */
static size_t fact_cap   = 0;       /* capacity of the cache          */

/* Ensure that fact_cache contains factorials up to k (inclusive). */
static void
ensure_factorial(idx_t k)
{
    if (fact_len > k)
        return;

    /* Increase capacity if needed. */
    if (fact_cap < (size_t)(k + 1)) {
        size_t new_cap = fact_cap ? fact_cap : 16;
        while (new_cap < (size_t)(k + 1))
            new_cap <<= 1;
        fact_cache = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }

    if (fact_len == 0) {
        fact_cache[0] = (seq_t)1;
        fact_len = 1;
    }

    for (idx_t i = fact_len; i <= k; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_len = k + 1;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined, but guard */

    /* Find the antidiagonal sum s such that s*(s–1)/2 ≥ n.  */
    idx_t s = 2;
    while (((__uint128_t)s * (s - 1) / 2) < n)
        ++s;

    /* Number of elements before this diagonal. */
    idx_t prev_total = (idx_t)((( __uint128_t)(s - 1) * (s - 2)) / 2);

    /* Position within the diagonal (1‑based). */
    idx_t pos = n - prev_total;

    idx_t m = pos;          /* row index  */
    idx_t k = s - pos;      /* column index */

    /* Range of factorial arguments needed: from s-2+1 to s-2+m. */
    idx_t max_fact = (s - 2) + m;
    ensure_factorial(max_fact);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t fact_idx = (s - 2) + i;
        sum += fact_cache[fact_idx];
    }

    return (seq_t)m * sum;
}
