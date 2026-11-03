/*
 * A142478  Primes congruent to 4 mod 51.
 * https://oeis.org/A142478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 919 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes ≡ 4 (mod 51) by incremental trial: a(n) is the
   n-th prime p with p % 51 == 4.  Uses a cache so that calls
   are amortised over many n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *cache = NULL;     /* cache of found primes */
static size_t cache_cap = 0;    /* allocated cache capacity */
static size_t cache_len = 0;    /* number of primes stored */
static idx_t next_candidate = 4;/* next value ≡ 4 (mod 51) to test */

/* Ensure the cache is large enough for at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < min_cap)
        new_cap <<= 1;
    /* realloc works for both C and C++; cast the returned void* */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1 – undefined for n = 0 */

    /* Generate primes until we have n primes in the cache. */
    while (cache_len < n) {
        /* Search for the next prime ≡ 4 (mod 51). */
        while (true) {
            if (next_candidate >= 2) {
                seq_t cand = (seq_t)next_candidate;
                if (isPrimeGeneric(cand)) {
                    grow_cache(cache_len + 1);
                    cache[cache_len++] = cand;
                    /* Advance to the following congruence class. */
                    next_candidate += 51;
                    break;
                }
            }
            next_candidate += 51;
        }
    }
    return cache[n - 1];
}
