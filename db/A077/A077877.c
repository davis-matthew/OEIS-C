/*
 * A077877  Expansion of (1-x)^(-1)/(1-x+2*x^2+x^3).
 * https://oeis.org/A077877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077877
   Expansion of (1-x)^(-1)/(1-x+2*x^2+x^3).
   Linear recurrence (for n ≥ 4):
       a(0) = 1, a(1) = 2, a(2) = 1, a(3) = -3
       a(n) = 2*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4)
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* static cache for memoised values */
static seq_t *cache = NULL;   /* array of computed a(n) */
static size_t cached = 0;     /* number of entries already computed (<= cap) */
static size_t cap    = 0;     /* current allocated capacity */

static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1U;
    if (cap >= needed) return;

    size_t new_cap = cap ? cap : 4U;
    while (new_cap < needed) new_cap <<= 1U;

#ifdef __cplusplus
    /* use the provided reallocator for non‑POD types */
    seq_t *tmp = seq_realloc(cache, cap, new_cap);
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)-3;

    /* ensure we have a cache large enough for indices up to n */
    ensure_capacity(n);

    /* make sure the first four entries are present */
    if (cached < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-3;
        cached = 4;
    }

    /* compute any missing terms up to n */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        cache[i] =  (seq_t)2 * cache[i-1]
                  - (seq_t)3 * cache[i-2]
                  +          cache[i-3]
                  +          cache[i-4];
    }
    cached = (size_t)n + 1U;
    return cache[(size_t)n];
}
