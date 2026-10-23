/*
 * A023368  a(n+1) = a(n) converted to base 5 from base 3 (written in base
 *          10).
 * https://oeis.org/A023368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 3; a(n+1) = interpret the base‑3 digits of a(n) as a
 *            base‑5 number (value returned in base 10)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   convert_base3_to_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/convert_base3_to_base5.h"

/* Cache for previously computed terms (1‑based, cache[0] unused). */
static seq_t *cache         = NULL;   /* array of seq_t values   */
static idx_t cache_len      = 0;      /* highest index that has been computed */
static size_t cache_cap     = 0;      /* current size of the allocated array */

/* Make sure we have space for n‑th term.  Reallocate if necessary. */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) exit(EXIT_FAILURE);        /* allocation error → abort */
        cache = p;
        cache_cap = new_cap;
    }
    if (cache_len == 0) {                     /* first time */
        cache[0] = 0;                         /* unused – keep it defined */
        cache[1] = (seq_t)3;                  /* a(1) */
        cache_len = 1;
    }
}

/* Public function that returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n = 0 */

    ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = convert_base3_to_base5(cache[i - 1]);
    }
    cache_len = n;
    return cache[n];
}
