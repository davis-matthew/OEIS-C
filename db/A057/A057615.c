/*
 * A057615  ATS: Add Then Sort (i.e., double previous term and then sort
 *          digits).
 * https://oeis.org/A057615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1, a(n) = sort_digits(2 * a(n-1)) for n > 1
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   sort_digits_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation does not rely on the external
 * helpers mul_small or sort_digits, but provides its own
 * lightweight digit‑sorting routine that works for any
 * unsigned integer type or arbitrary‑precision integer
 * class that supports the operators +, *, /, %.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sort_digits_local.h"

/* ============================ memoised sequence function ======================= */

/* static cache: index 1…n, cache[0] unused */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* allocated capacity, including unused index 0 */
static idx_t cache_len = 0;     /* highest n for which a(n) is known */

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n < 1 */
        return (seq_t)0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 8;                  /* start with 8 entries (0..7) */
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely in tests */
        cache_len = 0;
    }

    /* grow the cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap)
            new_cap <<= 1;
        /* guard against overflow */
        if ((size_t)new_cap > SIZE_MAX / sizeof(seq_t))
            new_cap = SIZE_MAX / sizeof(seq_t);

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)1;
        else
            cache[i] = sort_digits_local(cache[i - 1] * 2);
    }
    cache_len = n;

    return cache[n];
}
