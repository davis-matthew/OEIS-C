/*
 * A131570  a(1) = 1; for n >= 1, if a(n) is odd then a(n+1) = a(n) followed
 *          by a(n)+1, but if a(n) is even then a(n+1) = a(n)/2.
 * https://oeis.org/A131570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n ≥ 1, if a(n) is odd → a(n+1) = concat(a(n),
 *            a(n)+1) if a(n) is even → a(n+1) = a(n)/2 (concatenation is
 *            decimal)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   concat_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/concat_plus_one.h"

/* dynamic cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached elements            */
static size_t cache_cap = 0;   /* capacity of the cache array          */

/* ensure the cache can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* sequence function – external linkage */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* out of bounds – not used by specification */

    /* initialise cache with first term if empty */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = 1;        /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute terms up to n if required */
    if (cache_len < (size_t)n) {
        size_t target = (size_t)n;
        ensure_capacity(target);
        for (size_t i = cache_len; i < target; ++i) { /* i is 0‑based index */
            seq_t prev = cache[i - 1];
            seq_t next;
            if (prev & 1)      /* odd */
                next = concat_plus_one(prev);
            else                /* even */
                next = prev / 2;
            cache[i] = next;
        }
        cache_len = target;
    }

    return cache[(size_t)n - 1];
}
