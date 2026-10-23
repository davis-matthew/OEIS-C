/*
 * A126475  Number of base 7 n-digit numbers with adjacent digits differing by
 *          three or less.
 * https://oeis.org/A126475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4) (n>3) a(0) = 1,
 *            a(1) = 7, a(2) = 37, a(3) = 203
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t   cache_cap = 0;        /* allocated size of the array   */
static idx_t   cache_len = 0;        /* number of terms stored (cache_len-1 is the highest computed index) */

/* Ensure that the table contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)               /* already have enough */
        return;

    /* Allocate / enlarge space if needed.  We need at least 4 slots for the base cases. */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        if (new_cap < 4)
            new_cap = 4;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp)                    /* allocation failed – leave cache unchanged */
            return;
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        if (cache)
            free(cache);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if they are missing. */
    if (cache_len == 0) {
        static const seq_t base[4] = {1, 7, 37, 203};
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = base[i];
        cache_len = 4;
    }
    else if (cache_len < 4) {
        static const seq_t base[4] = {1, 7, 37, 203};
        for (idx_t i = cache_len; i < 4 && i <= n; ++i)
            cache[i] = base[i];
        idx_t need = (n < 3) ? (n + 1) : 4;
        if (cache_len < need)
            cache_len = need;
    }

    /* Compute remaining terms via the recurrence. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t term = 5 * cache[k - 1];
        term += 3 * cache[k - 2];
        term -= 2 * cache[k - 3];
        term -= cache[k - 4];
        cache[k] = term;
    }

    if (cache_len <= n)              /* update length after filling to n */
        cache_len = n + 1;
}

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
