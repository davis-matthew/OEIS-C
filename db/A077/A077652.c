/*
 * A077652  Primes whose initial and terminal decimal digits are identical.
 * https://oeis.org/A077652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 641, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   first_digit_uint64, last_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077652: primes whose initial and terminal decimal digits are identical.
   The function generates primes in order, keeping the cached list of primes that
   satisfy the digit‑matching property, so any call to a(n) (for any n ≥ 1)
   returns the nth term efficiently.  */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/first_digit_uint64.h"
#include "../utils/last_digit_uint64.h"

/* ------------- Prime test (Miller–Rabin, deterministic for 64‑bit) ------------- */

static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)   /* compilers that support __int128 */
    return (uint64_t)(((__uint128_t)a * b) % mod);
#else
    /* fallback: use 64‑bit arithmetic without overflow */
    uint64_t result = 0;
    while (b) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
#endif
}

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                     return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,41ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])  return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* Miller–Rabin with bases that are deterministic for 64‑bit integers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------- Helper: first and last decimal digits ------------- */

/* ------------- Cache for computed terms ------------- */

static seq_t *cache      = NULL;   /* dynamic array holding the sequence values   */
static size_t cache_size = 0;      /* number of terms currently stored           */
static size_t cache_cap  = 0;      /* allocated capacity of the cache             */
static uint64_t next_trial = 2;    /* next integer to test for primality          */

/* Ensure the cache has at least new_cap elements allocated. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t sz = cache_cap ? cache_cap : 16;
    while (sz < new_cap) sz <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    cache = tmp;
    cache_cap = sz;
}

/* ------------- Main function requested ------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n<1 is irrelevant */

    /* Grow the cache until it contains the requested term. */
    while (cache_size < n) {
        uint64_t cand = next_trial;
        /* Advance candidate: after 2   use odd numbers only */
        next_trial = (cand == 2) ? 3 : cand + 2;

        if (!is_prime_uint64(cand)) continue;

        uint8_t fd = first_digit_uint64(cand);
        uint8_t ld = last_digit_uint64(cand);
        if (fd == ld) {
            /* Store the prime in the cache. */
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }
    return cache[n-1];
}
