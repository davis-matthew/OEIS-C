/*
 * A162950  Primes whose square contains no repeated digit.
 * https://oeis.org/A162950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: full, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes whose square contains no repeated digit. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ---------- helper: check unique digits of a uint64_t ---------- */
static bool unique_digits_uint64(uint64_t v)
{
    unsigned mask = 0;
    while (v != 0)
    {
        unsigned d = v % 10;
        if (mask & (1U << d)) return false;
        mask |= (1U << d);
        v /= 10;
    }
    return true;
}

/* ---------- global state ---------- */
static seq_t *prime_cache = NULL;   /* primes that satisfy the property */
static size_t cache_count = 0;
static size_t cache_cap = 0;

static bool cache_initialized = false;

/* ---------- helper: grow cache ---------- */
static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 32 : cache_cap << 1;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp) return;  /* allocation failed – leave the cache incomplete */
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* ---------- prime sieve up to LIMIT ---------- */
#define LIMIT 100000  /* sqrt(9876543210) < 100000 */

static void init_cache(void)
{
    if (cache_initialized) return;
    cache_initialized = true;

    /* sieve of Eratosthenes on static array */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;  /* allocation failed – just leave cache empty */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= LIMIT; ++p)
    {
        if (is_prime[p])
        {
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    for (idx_t p = 2; p <= LIMIT; ++p)
    {
        if (!is_prime[p]) continue;

        /* add to list of primes that satisfy the property */
        uint64_t sq = (uint64_t)p * (uint64_t)p;
        if (unique_digits_uint64(sq))
        {
            if (cache_count == cache_cap) grow_cache();
            if (cache_cap == 0) break;  /* allocation failed earlier */
            prime_cache[cache_count++] = (seq_t)p;
        }
    }

    free(is_prime);
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */
    init_cache();
    return (n <= cache_count) ? prime_cache[n - 1] : (seq_t)0;
}
