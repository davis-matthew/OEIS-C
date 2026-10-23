/*
 * A034692  a(n+1) = smallest number that is not the sum of a(n) or fewer
 *          terms of a(1),...,a(n).
 * https://oeis.org/A034692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 + 2*a(n) – a(n)*a(n-1) – a(n-1) * (Recurrence
 *            given in the OEIS entry for A034692)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;   /* Array of cached terms.  */
static size_t capacity = 0;   /* Current allocated size.   */
static idx_t cached_n = 0;    /* Number of terms cached (n such that a(1)...a(n) are stored). */

/* Ensure that the cache can hold at least `needed` terms (1‑based).  */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();  /* Allocation failed – terminate. */
    cache = new_cache;
    capacity = new_cap;
}

/* Compute the n-th term of A034692 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Undefined for n=0; return 0 as placeholder. */

    /* Handle trivial cases. */
    if (n == 1)
        return 1;
    if (n == 2)
        return 2;

    /* Allocate and grow cache if necessary.  */
    ensure_cache_capacity(n);

    /* If we have already computed up to n, just return.  */
    if (n <= cached_n)
        return cache[n - 1];

    /* If cache is empty, seed base values.  */
    if (cached_n == 0) {
        cache[0] = 1;          /* a(1) */
        cache[1] = 2;          /* a(2) */
        cached_n = 2;
    }

    /* Compute missing terms using the recurrence.  */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t an   = cache[cached_n - 1];  /* a(k-1) */
        seq_t am1  = cache[cached_n - 2];  /* a(k-2) */
        /* a(k) = an^2 + 2*an – an*am1 – am1 */
        seq_t term = an * an          /* an^2  */
                 + 2 * an              /* + 2*an */
                 - an * am1            /* - an*am1 */
                 - am1;                /* - am1   */
        cache[cached_n++] = term;
    }

    return cache[n - 1];
}
