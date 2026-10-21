/*
 * A085837  Denominators of unit fractions having non-terminating decimal
 *          expansions.
 * https://oeis.org/A085837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = the nth positive integer that is NOT of the form 2^a*5^b
 *            (i.e. the nth denominator of a unit fraction with
 *            non‑terminating decimal)
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;          /* array of computed terms      */
static size_t cache_len = 0;         /* number of terms currently in cache */
static size_t cache_cap = 0;         /* allocated capacity              */
static seq_t last_value = 0;         /* last term produced (0 if none) */

/* Ensure the cache has room for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but guard */

    /* Generate terms up to index n (1‑based). */
    while ((idx_t)cache_len < n) {
        seq_t candidate = last_value + 1;

        /* Find the next integer that is NOT of the form 2^a*5^b. */
        while (1) {
            seq_t t = candidate;
            while (t % 2 == 0) t /= 2;
            while (t % 5 == 0) t /= 5;
            if (t != 1)          /* non‑terminating denominator */
                break;
            ++candidate;
        }

        ensure_capacity(cache_len + 1);
        cache[cache_len] = candidate;
        last_value = candidate;
        ++cache_len;
    }

    return cache[n - 1];
}
