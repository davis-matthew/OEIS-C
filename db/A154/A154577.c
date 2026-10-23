/*
 * A154577  Primes of the form 2n^2+14n+5.
 * https://oeis.org/A154577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the quadratic form 2k²+14k+5.
   The sequence is indexed from 1 as in OEIS A154577. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- local small Miller‑Rabin for 64‑bit numbers ---------- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U)
        return false;
    /* trial division for small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0ULL)
            return n == p;
    }
    /* Miller‑Rabin deterministic for 64‑bit integers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1U;
        ++s;
    }
    static const uint64_t test_bases[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL
    };
    for (size_t i = 0; i < sizeof test_bases / sizeof test_bases[0]; ++i) {
        uint64_t a = test_bases[i];
        if (a % n == 0ULL)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1ULL)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1ULL) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ----------  sequence implementation  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for 0; return 0 as placeholder */
        return (seq_t)0;

    /* static cache of already-computed values */
    static seq_t *cache   = NULL;   /* dynamic array of seq_t */
    static size_t   cap   = 0;      /* capacity of cache    */
    static size_t   len   = 0;      /* number of values stored */
    static idx_t   next_k = 0;      /* next k value to test   */

    /* if we already have the required term, return it */
    if (len >= (size_t)n)
        return cache[n - 1];

    /* grow the cache when necessary */
    if (len == cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* search for additional primes of the required form */
    for (idx_t k = next_k; ; ++k) {
        unsigned __int128 kk = (unsigned __int128)k;
        unsigned __int128 cand128 = 2U * kk * kk + 14U * kk + 5U;
        uint64_t cand64 = (uint64_t)cand128;   /* safe: values fit in 64‑bit for all n up to required limit */

        if (is_prime_uint64(cand64)) {
            cache[len++] = (seq_t)cand64;    /* implicit conversion works for built‑in and arbitrary­‑precision types */
            if (len >= (size_t)n) {
                next_k = k + 1;               /* last tested k + 1 */
                return cache[n - 1];
            }
        }
    }
}
