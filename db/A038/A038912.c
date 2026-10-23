/*
 * A038912  Primes p such that 35 is not a square mod p.
 * https://oeis.org/A038912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime p such that 35 is a quadratic non‑residue modulo p
   (OEIS sequence A038912).  The code searches for primes in increasing order
   and tests the Legendre symbol using Euler’s criterion.  It caches the
   computed terms so that calls to a() may be made in any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Small deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_small_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n == p)
            return true;
        if (n % p == 0ULL)
            return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        /* modular exponentiation */
        uint64_t result = 1;
        uint64_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL)
                result = (uint64_t)(((__uint128_t)result * base) % n);
            base = (uint64_t)(((__uint128_t)base * base) % n);
            exp >>= 1ULL;
        }
        if (result == 1ULL || result == n - 1)
            continue;
        bool ok = false;
        for (uint64_t r = 1; r < s; ++r) {
            result = (uint64_t)(((__uint128_t)result * result) % n);
            if (result == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok)
            return false;
    }
    return true;
}

/* Modular exponentiation (a^e mod mod) using 128‑bit multiplication. */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1ULL)
            res = (uint64_t)(((__uint128_t)res * a) % mod);
        a = (uint64_t)(((__uint128_t)a * a) % mod);
        e >>= 1ULL;
    }
    return res;
}

/* Return true iff 35 is NOT a quadratic residue modulo prime p. */
static bool not_quadratic_residue(uint64_t p)
{
    /* 2 and 3 are handled specially in the prime loop, but the test works
       for them as well. */
    return mod_pow_uint64(35ULL, (p - 1ULL) / 2ULL, p) != 1ULL;
}

/* --- cache management (static state) --- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1ULL;   /* last accepted prime (2 will follow) */

static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void produce_next(void)
{
    uint64_t candidate = last_checked + 1ULL;
    while (true) {
        if (candidate <= 2ULL)
            candidate = 2ULL;
        if (candidate == 2ULL || candidate == 3ULL) {
            break;  /* 2 and 3 are accepted */
        }
        if ((candidate & 1ULL) == 0ULL) {
            candidate += 1ULL;
            continue;
        }
        if (!is_prime_small_uint64(candidate)) {
            candidate += 2ULL;
            continue;
        }
        if (candidate == 5ULL || candidate == 7ULL) {
            candidate += 2ULL;
            continue;  /* 35 is zero mod 5 or 7, so reject */
        }
        if (not_quadratic_residue(candidate))
            break;  /* accept this prime */
        candidate += 2ULL;  /* next odd candidate */
    }
    ensure_cache(cache_len + 1);
    cache[cache_len++] = (seq_t)candidate;
    last_checked = candidate;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* unspecified for indices below offset */
    while (cache_len < (size_t)n)
        produce_next();
    return cache[(size_t)n - 1];
}
