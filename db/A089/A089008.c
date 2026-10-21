/*
 * A089008  Numbers k such that 18*k^2 + 1 is prime.
 * https://oeis.org/A089008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that 18*k^2+1 is prime
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a deterministic Miller–Rabin test for 64‑bit
 * numbers and caches found terms so that calls in arbitrary order are
 * efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Miller–Rabin primality test for 64‑bit numbers ---------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    /* even numbers are handled above */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is unused */
        return (seq_t)0;

    /* static cache state */
    static seq_t *cache   = NULL;  /* array of found k values */
    static size_t cap     = 0;     /* allocated length */
    static idx_t  found   = 0;     /* number of cached terms */
    static idx_t  next_k  = 1;     /* next candidate to test */

    while (found < n) {
        /* compute candidate k and test 18*k^2+1 */
        __int128 prod = (__int128)18 * next_k * next_k + 1;
        /* prod fits in uint64_t for all reasonably sized k used here */
        uint64_t val = (uint64_t)prod;

        if (is_prime_u64(val)) {
            /* grow cache if necessary */
            if (found == cap) {
                size_t new_cap = (cap == 0 ? 16 : cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)        /* allocation failed */
                    return (seq_t)0;
                cache = tmp;
                cap   = new_cap;
            }
            cache[found++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n-1];
}
