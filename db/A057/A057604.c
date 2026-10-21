/*
 * A057604  Primes of the form 4*k^2 + 163.
 * https://oeis.org/A057604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 94, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A057604 – Primes of the form 4*k^2 + 163
 * Definition: a(n) is the nth prime p such that p = 4*k^2 + 163 for some integer k ≥ 0.
 * The implementation keeps a lazily‑expanded cache of found primes and performs
 * successive tests of the form 4*k^2 + 163 using a 64‑bit primality routine.
 *
 * Note: The search is efficient enough for n ≲ 10 000 and works with
 * seq_t = uint8_t, uint16_t, uint32_t, uint64_t or uint128_t
 * as long as the numeric values stay within 64‑bit.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* Cached primes of the required form */
static seq_t *cache      = NULL;   /* array of found primes */
static size_t  cache_len = 0;      /* number of primes stored */
static size_t  cache_cap = 0;      /* current allocation size */
static uint64_t next_k     = 0;    /* next k to test */

/* Resize the cache when capacity is exhausted */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* unrecoverable memory error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n primes */
static void ensure_n(size_t n)
{
    while (cache_len < n) {
        /* Compute candidate 4*k^2 + 163 */
        uint64_t kk = next_k;
        uint64_t cand64 = 163ULL + 4ULL * kk * kk;
        /* Cast to seq_t – safe because cand64 fits in 64‑bit for all
           expected n (e.g., n ≤ 10 000 ⇒ cand ≈ 1.6×10¹¹). */
        seq_t cand = (seq_t)cand64;

        /* Test primality with the provided 64‑bit routine */
        if (is_prime_uint64(cand64)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = cand;
        }
        next_k++;        /* advance to the next k value */
    }
}

/* Return the nth term ( 1‑based index ) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless otherwise */
    ensure_n((size_t)n);
    return cache[n - 1];
}
