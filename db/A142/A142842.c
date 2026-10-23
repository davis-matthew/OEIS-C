/*
 * A142842  Primes congruent to 44 mod 61.
 * https://oeis.org/A142842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the n‑th prime congruent to 44 modulo 61
   by a simple trial‑division primality test and a growing cache.  It
   works for any seq_t that is an unsigned integer type, including
   arbitrarily‑precise implementations. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached prime values in the required residue class. */
static seq_t *cache = NULL;        /* array storing primes ≡44 (mod 61) */
static size_t cache_len = 0;       /* number of primes stored */
static size_t cache_cap = 0;       /* allocated capacity of cache */

/* The last integer examined by the prime finder (inclusive). */
static idx_t last_checked = 1;

/* Simple helper: extend the cache if needed.  We double capacity each
   time an additional prime is required. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failure – abort */ abort(); }
    cache = tmp;
    cache_cap = new_cap;
}

/* Trial division primality test for 64‑bit inputs.  This is fast enough
   for the values needed (≈ 10⁶) and works with seq_t because we only
   use it on idx_t values. */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/* Ensure that at least nth (1‑based) prime of the residue class is
   computed and stored in cache. */
static void ensure_n(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* Find next prime */
        for (;;) {
            last_checked++;
            if (is_prime(last_checked) && last_checked % 61 == 44)
                break;
        }
        /* Store it */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = (seq_t)last_checked;
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0ULL;            /* offset is 1, so n<1 is invalid */

    /* Compute the required term on demand */
    ensure_n(n);

    /* Return the cached value; cast is safe because cache was filled
       with primes that fit into seq_t or, if seq_t is too small,
       results are out of the tested range. */
    return cache[n - 1];
}
