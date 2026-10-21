/*
 * A208947  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A208947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical linear recurrence a(n) = 3*a(n‑1) -
 *            3*a(n‑2) + 2*a(n‑3) - 2*a(n‑4) + 2*a(n‑6) - 2*a(n‑7) + 3*a(n‑8)
 *            - 3*a(n‑9) + a(n‑10) with initial values a(1) … a(10) given
 *            below. This implements A208947 for all n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base_vals[10] =
    { 8, 68, 274, 766, 1722, 3376, 6004, 9928, 15514, 23178 };

static seq_t *cache = NULL;
static idx_t cache_max = 0;   /* largest n for which cache[n] is valid */

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but return 0 */

    /* Return if within initial range. */
    if (n <= 10)
        return base_vals[n - 1];

    /* Allocate or grow cache to hold up to n. */
    if (cache == NULL || cache_max < n)
    {
        idx_t new_size = n + 1;   /* indices 0..n; 0 unused */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) abort(); /* allocation failed */
        cache = new_cache;

        /* Initialize previously unused entries. */
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cache[i] = (seq_t)0;

        /* Fill base values if not already done. */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = base_vals[i - 1];

        cache_max = n;
    }

    /* Compute forward from the first index that needs updating. */
    for (idx_t i = 11; i <= n; ++i)
    {
        seq_t val =
            3 * cache[i - 1] + 2 * cache[i - 3] + 2 * cache[i - 6] +
            3 * cache[i - 8] + cache[i - 10];
        val -= 3 * cache[i - 2] + 2 * cache[i - 4] +
               2 * cache[i - 7] + 3 * cache[i - 9];
        cache[i] = val;
    }

    return cache[n];
}
