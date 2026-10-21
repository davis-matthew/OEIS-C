/*
 * A230442  Expansion of q^(-1/6) * eta(q)^2 * eta(q^2) in powers of q.
 * https://oeis.org/A230442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in ∏_{k>0} (1 - q^k)^2 (1 - q^{2k})
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A230442.  The function uses a
   static cache and recomputes the sequence up to the requested index
   when necessary.  The algorithm multiplies the generating function
   by each factor (1 - q^k)^2 (1 - q^{2k}) one at a time. */

#include <stdlib.h>

/* Forward declaration for internal recomputation routine */
static void recompute_to(idx_t n);

/* Static cache for previously computed coefficients */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements in cache */

/* Public entry point */
seq_t a(idx_t n)
{
    if (cache_len <= (size_t)n) /* need more terms */
        recompute_to(n);

    return cache[(size_t)n];
}

/* Recompute all terms up to index 'n' from scratch */
static void recompute_to(idx_t n)
{
    size_t N = (size_t)n + 1;           /* number of elements required   */
    seq_t *new_cache = (seq_t *)malloc(N * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed; leave cache unchanged */

    /* Initialize */
    new_cache[0] = 1;
    for (size_t i = 1; i < N; ++i)
        new_cache[i] = 0;

    /* Multiply by (1 - q^k)^2 (1 - q^{2k}) for each k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* First factor (1 - q^k) */
        for (idx_t m = n; m >= k; --m)
            new_cache[(size_t)m] -= new_cache[(size_t)(m - k)];

        /* Second factor (1 - q^k) */
        for (idx_t m = n; m >= k; --m)
            new_cache[(size_t)m] -= new_cache[(size_t)(m - k)];

        /* Factor (1 - q^{2k}) */
        idx_t twok = k << 1;
        for (idx_t m = n; m >= twok; --m)
            new_cache[(size_t)m] -= new_cache[(size_t)(m - twok)];
    }

    /* Replace old cache with new one */
    if (cache)
        free(cache);
    cache     = new_cache;
    cache_len = N;
}
