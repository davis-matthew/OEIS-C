/*
 * A190299  Squarefree semiprimes of the form 4k+1.
 * https://oeis.org/A190299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A190299: squarefree semiprimes of the form 4k+1.
   The value a(n) is obtained by enumerating integers m≥21,
   selecting those that are 1 (mod 4) and are the product of two
   distinct primes (i.e. square‑free semiprimes).  The sequence
   is cached so that subsequent calls with arbitrary n >= 1 are
   O(1) after the first expansion. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cached sequence values */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Cached prime list */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt   = 0;
static uint64_t  prime_limit = 0;   /* upper bound of generated primes */

/* Ensure we have primes up to at least `limit`. */
static void
ensure_primes(uint64_t limit)
{
    if (prime_limit >= limit)
        return;
    free(prime_list);
    build_primes_upto((idx_t)limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Test whether m is a square‑free semiprime ≡ 1 (mod 4). */
static bool
is_squarefree_semiprime(uint64_t m)
{
    /* Must be 1 (mod 4) and at least 3 × 7 (= 21). */
    if ((m & 3) != 1 || m < 21)
        return false;

    /* Find the smallest prime divisor of m. */
    ensure_primes(m);
    uint64_t p = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i) {
        if (m % prime_list[i] == 0) {
            p = prime_list[i];
            break;
        }
    }
    if (p == 0)          /* m is prime */
        return false;

    uint64_t q = m / p;
    if (p == q)          /* square factor */
        return false;

    /* q may exceed sqrt(m); test primality directly. */
    return is_prime((idx_t)q);
}

/* Append a new term to the cached sequence. */
static void
append_term(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap << 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* Required function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* Expand the cache until it contains n terms. */
    static uint64_t candidate = 20;  /* last number tested */
    while ((idx_t)cache_len < n) {
        ++candidate;
        if (is_squarefree_semiprime(candidate))
            append_term((seq_t)candidate);
    }
    return cache[n - 1];
}
