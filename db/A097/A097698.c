/*
 * A097698  Numbers k such that both 4*k^2 - 3 and 4*k^2 + 3 are primes.
 * https://oeis.org/A097698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th integer k such that 4*k^2-3 and 4*k^2+3 are
 *            both prime (offset 1)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool isPrimeUint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        uint64_t p = smallPrimes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int r = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++r;
    }
    /* Bases sufficient for deterministic test for n < 2^64. */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        /* Compute a^d mod n */
        unsigned __int128 x = 1, power = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }
        uint64_t xy = (uint64_t)x;
        if (xy == 1 || xy == n - 1) continue;
        bool composite = true;
        for (int j = 1; j < r; ++j) {
            x = (x * x) % n;
            xy = (uint64_t)x;
            if (xy == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Static storage for computed terms. */
static seq_t *term_cache = NULL;   /* array of found k's */
static size_t term_cache_size = 0;
static size_t term_cache_cap = 0;
static uint64_t next_k = 1;        /* next candidate k to test */

/* Append a new term to the cache, expanding storage if necessary. */
static void cache_push(uint64_t k)
{
    if (term_cache_cap == 0) {
        term_cache_cap = 16;
        term_cache = (seq_t*)malloc(sizeof(seq_t) * term_cache_cap);
    } else if (term_cache_size == term_cache_cap) {
        term_cache_cap *= 2;
        term_cache = (seq_t*)realloc(term_cache, sizeof(seq_t) * term_cache_cap);
    }
    term_cache[term_cache_size++] = (seq_t)k;
}

/* Main sequence function.  It expands the cache until it contains at least n terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe fallback */

    while ((size_t)n > term_cache_size) {
        uint64_t k = next_k++;          /* candidate k */
        unsigned __int128 tmp = (__int128)k * k;
        tmp *= 4;                       /* 4*k^2 */
        uint64_t val1 = (uint64_t)(tmp - 3);
        uint64_t val2 = (uint64_t)(tmp + 3);
        if (isPrimeUint64(val1) && isPrimeUint64(val2))
            cache_push(k);
    }
    return term_cache[n - 1];
}
