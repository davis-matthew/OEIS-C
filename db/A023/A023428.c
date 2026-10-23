/*
 * A023428  Generalized Catalan Numbers x^4*A(x)^2 -(1-x+x^4+x^5)*A(x) +1 =0.
 * https://oeis.org/A023428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A023428: Generalized Catalan Numbers
 *
 * Recurrence:
 *   a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = 1
 *   a(n) = a(n-1) + Σ_{k=2}^{n-4} a(k)·a(n-4-k)   for n ≥ 6
 *
 * This implementation memoizes computed terms in a static array so that
 * calls can arrive in any order.  Dynamic array growth is performed with
 * realloc; no external helpers are required.
 */

#include <stddef.h>
#include <stdlib.h>

static void
ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0 ? 16 : (*cap << 1));
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Memory allocation failure – abort to keep behaviour defined */
        abort();
    }
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;  /* number of terms actually initialized */

    /* Ensure storage for indices up to n */
    ensure_capacity((size_t)n + 1, &cache_cap, &cache);

    /* Initialize first six terms once */
    if (cache_len == 0) {
        /* Make sure we have a capacity for at least 6 slots */
        if (cache_cap < 6)
            ensure_capacity(6, &cache_cap, &cache);
        for (idx_t i = 0; i <= 5; ++i)
            cache[i] = (seq_t)1;
        cache_len = 6;
    }

    /* If the requested term is already available, return it */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* Compute terms from cache_len up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* For m <= 5 the values are already set to 1 */
        if (m <= 5) {
            cache[(size_t)m] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 2; k <= m - 4; ++k) {
            sum += cache[(size_t)k] * cache[(size_t)(m - 4 - k)];
        }
        cache[(size_t)m] = cache[(size_t)(m - 1)] + sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
