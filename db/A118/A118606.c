/*
 * A118606  Start with 1 and repeatedly reverse the digits and add 17 to get
 *          the next term.
 * https://oeis.org/A118606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = reverse_decimal(a(n‑1)) + 17   (n ≥ 2) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_decimal.h"

static seq_t *cache = NULL;          /* 1‑based cache, cache[0] unused */
static idx_t max_cached = 0;         /* largest n for which cache[n] is known */

seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1; behaviour below offset is irrelevant */
        return (seq_t)0;

    /* First invocation: allocate up to the requested n and initialise all terms. */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* allocation failure */
        /* a(1)=1 */
        cache[1] = (seq_t)1;
        idx_t i;
        for (i = 2; i <= n; ++i)
            cache[i] = reverse_decimal(cache[i - 1]) + (seq_t)17;
        max_cached = n;
        return cache[n];
    }

    /* If the requested n is already in the cache, return it immediately. */
    if (n <= max_cached)
        return cache[n];

    /* Extend the cache to include the new term(s). */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return (seq_t)0;              /* allocation failure */
    cache = new_cache;
    idx_t i;
    for (i = max_cached + 1; i <= n; ++i)
        cache[i] = reverse_decimal(cache[i - 1]) + (seq_t)17;
    max_cached = n;
    return cache[n];
}
