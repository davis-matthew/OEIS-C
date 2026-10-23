/*
 * A167646  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) for n ≥ 0 – rational GF with order‑14 linear recurrence
 *            (a(0) … a(13) given). Recurrence derived from G(t) = (t^14 +
 *            2t^13 + … + 1) / (1128 t^14 – 47 t^13 – … – 47 t + 1) yields for
 *            n ≥ 14: a(n) = 47·a(n‑1) + … + 47·a(n‑13) – 1128·a(n‑14) For n =
 *            14 the numerator contributes an extra +1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Arbitrary‑precision literal parser provided by the build system. */
#include "../utils/seq_from_str.h"

static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Initial 14 terms (indices 0..13).  Provided from the OEIS b‑file. */
static const char * const init_strs[14] = {
    "1",
    "49",
    "2352",
    "112896",
    "5419008",
    "260112384",
    "12485394432",
    "599298932736",
    "28766348771328",
    "1380784741023744",
    "66277667569139712",
    "3181328043318706176",
    "152703746079297896448",
    "7329779811806299029504"
};

/* Initialise the static cache with the first 14 terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                       /* already initialised */

    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        abort();

    for (size_t i = 0; i < 14; ++i)
        cache[i] = seq_from_str(init_strs[i]);

    cache_len = 14;
}

/* Extend the cache so that it contains values up to index 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len > n)
        return;

    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i < need; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 13; ++k)
            sum += cache[i - k];
        sum = sum * (seq_t)47;
        seq_t term = sum - cache[i - 14] * (seq_t)1128;
        if (i == 14)                 /* numerator contributes +1 at n = 14 */
            term += 1;
        cache[i] = term;
    }

    cache_len = need;
}

/* Sequence function requested by the harness. */
seq_t a(idx_t n)
{
    init_cache();
    ensure_cache(n);
    return cache[(size_t)n];
}
