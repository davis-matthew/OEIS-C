/*
 * A107644  Numbers n such that the string 1113n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A107644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Series: a(n) = the n-th integer k such that the decimal
   concatenation of "1113" and the decimal representation of k
   is a prime number.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 0‑based helpers ---------- */

/* Pre‑computed powers of 10 up to 10¹⁸ (fits in uint64_t). */
static const uint64_t pow10[19] = {
    1ULL,
    10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL
};

/* Count decimal digits of a uint64_t (fast, no branch for small numbers). */
static int digit_count_uint64(uint64_t x)
{
    int d = 1;
    if (x >= 1000000000000000000ULL) return d + 18;
    if (x >= 100000000000000000ULL)  return d + 17;
    if (x >= 10000000000000000ULL)   return d + 16;
    if (x >= 1000000000000000ULL)    return d + 15;
    if (x >= 100000000000000ULL)     return d + 14;
    if (x >= 10000000000000ULL)      return d + 13;
    if (x >= 1000000000000ULL)       return d + 12;
    if (x >= 100000000000ULL)        return d + 11;
    if (x >= 10000000000ULL)         return d + 10;
    if (x >= 1000000000ULL)          return d + 9;
    if (x >= 100000000ULL)           return d + 8;
    if (x >= 10000000ULL)            return d + 7;
    if (x >= 1000000ULL)             return d + 6;
    if (x >= 100000ULL)              return d + 5;
    if (x >= 10000ULL)               return d + 4;
    if (x >= 1000ULL)                return d + 3;
    if (x >= 100ULL)                 return d + 2;
    if (x >= 10ULL)                  return d + 1;
    return d;                        /* 0..9 */
}

/* Modular multiplication using 128‑bit to avoid overflow. */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Modular exponentiation (a^e mod mod). */
static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for uint64_t.  Bases 2,325,9375,28178,450775,9780504,1795265022
   are sufficient for all 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
        9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; r++) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- 1‑based sequence implementation ---------- */

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size */
static idx_t  cache_sz = 0;       /* number of stored terms */
static idx_t  last_checked = 0;   /* last integer examined */

/* Ensure 'cache' has at least 'new_cap' elements. */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t grow = cache_cap ? cache_cap : 16;
    while (grow < new_cap) grow <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, grow * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = grow;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Return cached value if already computed. */
    if (n <= cache_sz) return cache[n - 1];

    /* Extend cache as needed. */
    ensure_cache_cap((size_t)(n * 2));

    /* Generate terms until the n-th one is found. */
    while (cache_sz < n) {
        last_checked++;

        int d = digit_count_uint64(last_checked);
        uint64_t mult = pow10[d];          /* 10^d */

        /* Build candidate prime: 1113 * 10^d + last_checked. */
        __uint128_t cand128 = ( (__uint128_t)1113 * mult ) + last_checked;
        if (cand128 > (__uint128_t)UINT64_MAX) continue;   /* skip overflow */

        uint64_t candidate = (uint64_t)cand128;
        if (is_prime_uint64(candidate)) {
            cache[cache_sz++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
