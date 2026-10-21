/*
 * A050076  a(n) = floor(a(n-1)/3) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 2*a(n-1).
 * https://oeis.org/A050076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(1) = 1 for n ≥ 2: let t =
 *            floor(a(n‑1) / 3) if t = 0 or t has appeared earlier, a(n) = 2 *
 *            a(n‑1) otherwise a(n) = t This yields a permutation of the
 *            natural numbers
 * Range:     u8 n <= 24, u16 n <= 1209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Dynamic cache of already-computed terms (1‑based indexing). */
static seq_t *cache      = NULL;   /* array of computed a(n) */
static size_t cache_len  = 0;      /* number of computed entries */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure that 'cache' can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return true if 'val' appears in cache[0 .. cache_len-1]. */
static bool used_before(seq_t val)
{
    for (size_t i = 0; i < cache_len; ++i) {
        if (cache[i] == val)
            return true;
    }
    return false;
}

/* Main function requested by the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0; return 0 safely */
        return 0;

    /* Convert n to a size_t for array indexing. */
    size_t idx = (size_t)n;

    /* One‑time initialization on first call. */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* Produce terms up to the requested index (if not already computed). */
    while (cache_len < idx) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev / (seq_t)3;   /* floor division */

        bool cand_used = false;
        if (cand == (seq_t)0)
            cand_used = true;          /* 0 is never allowed */
        else if (used_before(cand))
            cand_used = true;

        seq_t next = cand_used ? prev * (seq_t)2 : cand;

        /* Append the new term to the cache. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[idx - 1];
}
