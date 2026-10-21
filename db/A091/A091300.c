/*
 * A091300  Nonprimes of the form 6k + 1.
 * https://oeis.org/A091300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth nonprime (or 1) that is ≡ 1 (mod 6). a(1) = 1
 * Range:     u8 n <= 18, u16 n <= 7664, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    __uint128_t r = 1;
    __uint128_t b = a % mod;
    while (e) {
        if (e & 1) r = r * b % mod;
        b = b * b % mod;
        e >>= 1;
    }
    return (uint64_t)r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division for small primes */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* deterministic Miller–Rabin for 64‑bit integers */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* Bases that are sufficient for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (uint64_t)((__uint128_t)x * x % n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_k   = 0;   /* last k used (for 6k+1) */

    /* grow cache if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n */
    while (cache_size < (size_t)n) {
        if (cache_size == 0) {             /* first term is 1 */
            cache[0] = (seq_t)1;
            cache_size = 1;
            last_k = 0;                     /* next candidate will be 7 */
            continue;
        }

        /* find next 6k+1 that is NOT prime */
        for (;;) {
            ++last_k;
            uint64_t cand = 6ULL * last_k + 1ULL;
            if (!is_prime_uint64(cand)) {
                cache[cache_size++] = (seq_t)cand;
                break;
            }
        }
    }

    return cache[n - 1];
}
