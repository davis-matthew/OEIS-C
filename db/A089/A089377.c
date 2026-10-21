/*
 * A089377  Numbers whose product of digits +1 and -1 gives twin prime pairs.
 * https://oeis.org/A089377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 7100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mulmod64, seq_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089377: Numbers whose product of digits ±1 gives a twin prime pair.
   Implementation: incremental search with a deterministic 64‑bit Miller–Rabin
   primality test. For each candidate integer the product of its decimal digits
   is computed; if both prod−1 and prod+1 are prime, the candidate is a term
   of the sequence. Cached results allow direct access to any a(n) for n≥1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_grow.h"     /* includes the seq_grow helper */
#include "../utils/mulmod64.h"

static inline uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1U)
            result = mulmod64(result, b, mod);
        b = mulmod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2)            return false;
    static const uint64_t small_primes[] = {2U,3U,5U,7U,11U,13U,17U,19U,23U,29U,31U,37U};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1U) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1: undefined for n < 1 */

    /* static cache for discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;   /* next integer to test */

    /* allocate initial cache if needed */
    if (cache == NULL) {
        seq_grow(&cache, &cache_len, &cache_cap);
    }

    /* generate terms until we have at least n terms */
    while (cache_len < n) {
        /* product of decimal digits of next_candidate */
        uint64_t prod = 1;
        uint64_t x = next_candidate;
        if (x == 0) {
            prod = 0;
        } else {
            while (x > 0) {
                prod *= x % 10;
                x /= 10;
            }
        }

        /* test twin prime condition */
        if (prod > 0 && is_prime64(prod - 1) && is_prime64(prod + 1)) {
            seq_grow(&cache, &cache_len, &cache_cap);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
