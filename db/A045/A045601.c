/*
 * A045601  Numbers k that divide 8^k + 6^k.
 * https://oeis.org/A045601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045601: numbers k such that k | 8^k + 6^k. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Cached sequence values (1‑based indexing). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_k     = 1;           /* next k to test         */

/* Ensure capacity for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);            /* OOM – terminate */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Append a found term to the cache. */
static void add_term(seq_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        idx_t k = next_k++;
        seq_t modk = (seq_t)k;

        seq_t pow8 = mod_pow((seq_t)8, k, modk);
        seq_t pow6 = mod_pow((seq_t)6, k, modk);

        seq_t sum = pow8 + pow6;
        if (sum >= modk) sum -= modk;

        if (sum == (seq_t)0)
            add_term(modk);
    }
    return cache[n - 1];
}
