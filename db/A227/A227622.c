/*
 * A227622  Primes p of the form m^2 + 27.
 * https://oeis.org/A227622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes of the form m^2 + 27, a(n) = the nth such prime
 * (OEIS A227622).  The function searches for primes p = m² + 27,
 * caching results for subsequent calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t   *prime_cache   = NULL; /* array of primes found */
static size_t  cache_cap      = 0;     /* allocated size of prime_cache */
static size_t  cache_len      = 0;     /* number of primes stored */
static uint64_t next_m        = 1;     /* next m to test (m >= 1) */

/* Expand the cache to hold at least 'needed' elements. */
static void
grow_cache_to(size_t needed)
{
    if (needed <= cache_cap)
        return;
    ensure_capacity(needed, &cache_cap,
                    (void **)&prime_cache, sizeof(seq_t));
}

/* Generate primes of the form m^2 + 27 until we have 'desired' primes. */
static void
ensure_primes(size_t desired)
{
    while (cache_len < desired) {
        seq_t p = (seq_t)next_m * (seq_t)next_m + (seq_t)27;
        if (is_prime_big(p))
            grow_cache_to(cache_len + 1), prime_cache[cache_len++] = p;
        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1, but avoid underflow */
        return 0;
    size_t idx = (size_t)n;     /* n fits in size_t for practical limits */
    if (idx == 0)
        return 0;
    --idx;                      /* adjust for 0‑based array indexing */
    ensure_primes(idx + 1);
    return prime_cache[idx];
}
