/*
 * A156323  List of prime pairs of the form (p, p+12).
 * https://oeis.org/A156323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the primes p and p+12 alternately, where both p * and
 *            p+12 are prime. The sequence starts 5, 17, 7, 19, 11, 23, …
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence lazily, storing the
 * computed terms in a resizable cache.  It uses the helper
 * is_prime_big (provided in ../utils/is_prime_big.h) for
 * prime testing; this routine works for all supported types of
 * seq_t, including arbitrary‑precision big integers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache    = NULL;   /* array of terms already computed   */
static size_t cache_len = 0;     /* number of cached terms            */
static size_t cache_cap = 0;     /* current allocated capacity        */

static seq_t next_candidate = 5; /* first odd number to examine       */

/* Resize the cache so that it can hold at least `need` more elements. */
static void grow_cache(size_t need)
{
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 64;
    while (new_cap < cache_len + need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache   = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache, expanding it if necessary. */
static void cache_append(seq_t v)
{
    grow_cache(1);
    cache[cache_len++] = v;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) abort();     /* sequence is 1‑indexed */

    /* Generate terms until we have at least *n* cached. */
    while (cache_len < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            seq_t p12 = next_candidate + (seq_t)12;
            if (is_prime_big(p12)) {
                cache_append(next_candidate); /* p   */
                cache_append(p12);             /* p+12 */
            }
        }
        next_candidate += (seq_t)2;          /* next odd number */
    }

    return cache[(size_t)n - 1];
}
