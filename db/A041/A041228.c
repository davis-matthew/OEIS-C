/*
 * A041228  Numerators of continued fraction convergents to sqrt(126).
 * https://oeis.org/A041228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   _cf_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 11;
   for n ≥ 1: a(n) = c(n) · a(n‑1) + a(n‑2)   where
     c(n) = 4, 2, 4, 22   and repeats every 4 indices. */
#include <stdlib.h>
#include "../utils/_cf_coeff.h"

/* Cache of already computed numerators. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;

/* Numerators of continued‑fraction convergents to sqrt(126). */
seq_t a(idx_t n)
{
    /* If already cached, return immediately. */
    if (n < cache_len) return cache[(size_t)n];

    /* Grow cache to at least n + 1 elements. */
    size_t need = (size_t)n + 1;
    if (need > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation fail */
        cache = tmp;

        /* Ensure the base value a(0) is set. */
        if (cache_len == 0) {
            cache[0] = (seq_t)11;
            cache_len = 1;
        }

        /* Previous two terms needed for the recurrence. */
        seq_t prev2, prev1;
        if (cache_len == 1) {
            /* Only a(0) computed so far: p_{-1}=1, p_0=a(0). */
            prev2 = (seq_t)1;
            prev1 = cache[0];
        } else {
            prev2 = cache[cache_len - 2];
            prev1 = cache[cache_len - 1];
        }

        /* Compute missing terms up to n. */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t coeff = _cf_coeff(i);
            seq_t cur   = coeff * prev1 + prev2;
            cache[i]    = cur;
            prev2 = prev1;
            prev1 = cur;
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
