/*
 * A129216  Numbers m such that 10!*m-/+1 are twin primes.
 * https://oeis.org/A129216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1920, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime searching definition: m such that 10!*m ± 1 are twin primes.
   The function returns the nth term of OEIS A129216 (1‑based). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* 10! as an unsigned 64-bit value. (No risk of overflow in 64 bits.) */
static const uint64_t FACT_TEN = 3628800ULL;

/* 64‑bit modular multiplication (uses 128‑bit intermediate for safety). */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation. */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool witness = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                witness = false;
                break;
            }
        }
        if (witness)
            return false;
    }
    return true;
}

/* Cache of computed terms. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_m    = 1;   /* next m to test */

/* Expand the cache until it contains at least 'n' terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t prod = FACT_TEN * (uint64_t)next_m;
        if (is_prime_uint64(prod - 1) && is_prime_uint64(prod + 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0; return 0 as sentinel */
    ensure_cache(n);
    return cache[n - 1];   /* a(1) is stored at cache[0] */
}
