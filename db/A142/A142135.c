/*
 * A142135  Primes congruent to 26 mod 37.
 * https://oeis.org/A142135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   compute primes congruent to 26 (mod 37) in ascending order.
 *            Implementation strategy: - The sequence is defined as the
 *            increasing list of primes p such that p ≡ 26 (mod 37). The first
 *            such prime is 137. - We generate the sequence lazily: a cache of
 *            found primes is kept statically. The function `ex...
 * Range:     u8 n <= 2, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"   /* uint64_t is_prime_small(uint64_t) */

static seq_t *cache      = NULL;   /* cached primal progression  */
static size_t cache_cap  = 0;      /* capacity of cache          */
static size_t cache_len  = 0;      /* number of stored terms     */

static uint64_t next_candidate = 26ULL;   /* next number of the form 26+37*k */

/* Grow cache to at least `need` elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Append value to cache, growing it if necessary. */
static void append_to_cache(seq_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate primes of the form 26 (mod 37) until `cache_len` ≥ `req_n`. */
static void expand_to_n(idx_t req_n)
{
    while (cache_len < (size_t)req_n) {
        next_candidate += 37ULL;                     /* next congruent candidate */
        /* Test only integers that fit in uint64_t, safe for the range needed. */
        if (is_prime_small(next_candidate))
            append_to_cache((seq_t)next_candidate);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset is 1 */
    if (cache_len < (size_t)n) expand_to_n(n);
    return cache_len >= (size_t)n ? cache[n - 1] : 0;
}
