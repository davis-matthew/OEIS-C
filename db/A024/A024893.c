/*
 * A024893  Numbers k such that 3*k+2 is prime.
 * https://oeis.org/A024893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A024893: Numbers k such that 3*k+2 is prime. 
   Computed by brute‑force search with memoisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_modpow_uint64.h"

static bool _is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;
    static const uint64_t primes[] = {
        2U,3U,5U,7U,11U,13U,17U,19U,23U,29U,31U,37U
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        if (n % primes[i] == 0)
            return n == primes[i];
    }
    uint64_t d = n - 1U;
    uint64_t s = 0U;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0U) continue;
        uint64_t x = _modpow_uint64(a, d, n);
        if (x == 1U || x == n - 1U) continue;
        bool composite = true;
        for (uint64_t r = 1U; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1U) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Static cache for memoisation */
static seq_t *cache  = NULL;   /* sequence values (k) */
static idx_t  size   = 0;      /* number of cached elements */
static idx_t  cap    = 0;      /* allocated capacity */
static idx_t  next   = 0;      /* next candidate k to test */

static void _ensure_capacity(idx_t needed)
{
    if (cap >= needed) return;
    idx_t newcap = (cap == 0) ? 8 : (cap << 1);
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    cache = tmp;
    cap   = newcap;
}

/* Public function. External linkage is implicit. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1 – behaviour for n<1 is irrelevant */

    while (size < n) {
        idx_t cand = next++;        /* candidate k */
        /* 3*k+2 fits in 64-bit for the ranges used. */
        uint64_t val = (uint64_t)3 * cand + 2ULL;
        if (_is_prime_uint64(val)) {
            _ensure_capacity(size + 1);
            cache[size++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
