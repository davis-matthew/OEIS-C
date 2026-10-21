/*
 * A200959  Primes of the form 8n^3-9.
 * https://oeis.org/A200959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 8*k^3-9 (k ≥ 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 117, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

/* Deterministic Miller–Rabin for uint64_t */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        uint64_t x = 1;
        uint64_t base = a;
        uint64_t exp = d;
        /* binary exponentiation with 128‑bit multiplication */
        while (exp) {
            if (exp & 1) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* 0‑based cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* last k processed (exclusive) – start before the first valid k */
    static idx_t last_k = 3;               /* next k will start at 4 → 503 */

    /* expand cache until it contains at least n primes */
    while (cache_len < n) {
        ++last_k;

        unsigned __int128 k128 = (unsigned __int128)last_k;
        unsigned __int128 v128 = 8 * k128 * k128 * k128 - 9;

        /* If seq_t is 64‑bit and the value would overflow, stop. */
        if (sizeof(seq_t) <= 8 && v128 > (unsigned __int128)UINT64_MAX)
            break;

        /* Convert to seq_t (automatic conversion for types larger
           than 128‑bit is implementation defined but acceptable for
           the intended builds). */
        seq_t val = (seq_t)v128;

        /* Test primality: 64‑bit build uses our fast routine,
           arbitrary‑precision uses the supplied big test. */
        if (sizeof(seq_t) <= 8) {
            if (is_prime_uint64((uint64_t)val))
                append_candidate(&cache, &cache_len, &cache_cap, val);
        } else {
            if (is_prime_big(val))
                append_candidate(&cache, &cache_len, &cache_cap, val);
        }
    }

    /* Return the n‑th prime (1‑based index). */
    return cache[n - 1];
}
