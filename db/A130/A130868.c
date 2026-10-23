/*
 * A130868  Numbers k such that the set of digits of k^2 is the same as the
 *          set of digits of (k+1)^2.
 * https://oeis.org/A130868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130868: numbers k such that the set of digits of k^2
   equals the set of digits of (k+1)^2.
   Computed by scanning k incrementally and caching the matches. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Convert the set of decimal digits contained in v into a 10‑bit mask: */
static unsigned int digit_mask(seq_t v)
{
    unsigned int mask = 0u;
    if (v == (seq_t)0) return 1u << 0;
    while (v != (seq_t)0) {
        unsigned int d = (unsigned int)(v % (seq_t)10);
        mask |= 1u << d;
        v /= (seq_t)10;
    }
    return mask;
}

/* Does the integer k satisfy the condition? */
static bool matches_k(seq_t k)
{
    seq_t sq1 = k * k;
    seq_t sq2 = (k + (seq_t)1) * (k + (seq_t)1);
    return digit_mask(sq1) == digit_mask(sq2);
}

/* Cached results – only 64‑bit unsigned integers are stored.  Casting to
   seq_t is safe for both built‑in integer types and big‑integer classes. */
static uint64_t *cache_vals   = NULL;   /* array of matched k values   */
static size_t cache_cap       = 0;      /* capacity of cache_vals      */
static size_t cache_len       = 0;      /* number of entries stored    */
static uint64_t next_candidate = 1;     /* smallest k not yet examined */

/* Ensure that the cache has room for at least 'need' entries.  */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (newcap < need) newcap *= 2;
    void *p = realloc(cache_vals, newcap * sizeof(uint64_t));
    if (!p) exit(EXIT_FAILURE);     /* in practice handle error properly */
    cache_vals = (uint64_t *)p;
    cache_cap = newcap;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n==0, return 0 */

    while (cache_len < (size_t)n) {
        uint64_t k = next_candidate;
        if (matches_k((seq_t)k))
            ensure_cache_cap(cache_len + 1),
            cache_vals[cache_len++] = k;
        next_candidate = k + 1;
    }
    return (seq_t)cache_vals[n - 1];
}
