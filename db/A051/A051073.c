/*
 * A051073  Primes p such that x^8 = -2 has a solution mod p.
 * https://oeis.org/A051073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that x^8 ≡ -2 (mod p) has a solution
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 983 (2026-11-03)
 * Helpers:   gcd_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* ---------- modular exponentiation for 128‑bit values ---------- */
static unsigned __int128 pow_mod128(unsigned __int128 base,
                                     unsigned long long exp,
                                     unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Predicate: does p admit a solution to x^8 ≡ -2 (mod p)? */
static inline bool has_solution(uint64_t p) {
    if (p == 2) return true;                   /* 0^8 ≡ -2 ≡ 0 (mod 2) */

    uint64_t g   = gcd_u64(8ULL, p - 1);       /* gcd(8, p-1) */
    uint64_t exp = (p - 1) / g;                /* exponent for the test */
    unsigned __int128 base = (unsigned __int128)(p - 2);

    unsigned __int128 val = pow_mod128(base, exp, (unsigned __int128)p);
    return val == 1;
}

/* Cached list of qualifying primes. */
static seq_t   *cache   = NULL;     /* array of seq_t values */
static size_t  cache_len = 0;       /* number of stored primes */
static size_t  cache_cap = 0;       /* allocated capacity of the array */
static uint64_t last_prime = 1;     /* last prime that was examined */

/* Ensure the cache can hold at least the requested number of elements. */
static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) return;                     /* allocation failed – keep old */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n = 0 */
    while (cache_len < (size_t)n) {
        last_prime = next_prime(last_prime);
        if (has_solution(last_prime)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_prime;
        }
    }
    return cache[n - 1];
}
