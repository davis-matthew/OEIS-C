/*
 * A064466  a(0) = 6 and a(n) = Min { m > a(n-1) | both a(n-1) - p and m - p
 *          are prime for some prime p } for n > 0.
 * https://oeis.org/A064466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(0) = 6 a(n) = min{ m > a(n-1) | ∃ prime p:
 *            a(n-1)-p and m-p are prime } The function memoises previously
 *            computed terms
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Find the first prime strictly greater than n.  Works for arbitrary-precision seq_t. */
static seq_t next_prime_big(seq_t n)
{
    seq_t cand = n + 1;
    if (cand <= 2)
        return 2;
    if (cand % 2 == 0)         /* make odd */
        cand += 1;
    for (;;) {
        if (is_prime_big(cand))
            return cand;
        cand += 2;
    }
}

/* Compute a(n) from a(n‑1). */
static seq_t compute_next(seq_t prev)
{
    seq_t best_m = 0;
    /* p must be prime and <= prev-2 */
    for (seq_t p = 2; p <= prev - 2; p = next_prime_big(p)) {
        seq_t q = prev - p;            /* q must be prime */
        if (!is_prime_big(q))
            continue;
        seq_t r = next_prime_big(q);   /* minimal prime > q */
        seq_t m_candidate = p + r;      /* candidate m */
        if (best_m == 0 || m_candidate < best_m)
            best_m = m_candidate;
    }
    /* Fallback, should never be needed for valid inputs */
    return best_m ? best_m : prev + 2;
}

/* ----- Memoisation cache ----- */
static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored (>= cache_cap) */

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;               /* assume realloc doesn't fail */
        cache_cap = new_cap;
    }

    if (cache_len == 0) {
        cache[0] = 6;              /* a(0) */
        cache_len = 1;
    }

    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        cache[(size_t)i] = compute_next(cache[(size_t)(i - 1)]);
    }
    cache_len = (size_t)(n + 1);
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
