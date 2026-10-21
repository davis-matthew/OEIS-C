/*
 * A089358  Numbers k such that k^2 - 3*k + 3 is prime.
 * https://oeis.org/A089358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer k with k^2 - 3 k + 3 prime
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If k^(2) - 3 k + 3 is prime then k is included in the sequence.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Deterministic Miller‑Rabin primality test for 64‑bit integers.      */
/* ------------------------------------------------------------------ */
static uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d > 0) {
        if (d & 1) r = mulmod_uint64(r, a, mod);
        a = mulmod_uint64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool isPrime(uint64_t n)
{
    if (n < 2) return false;
    /* Small prime checks */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller‑Rabin with deterministic bases for 64‑bit integers */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Stores found terms in a static cache.     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t count = 0;
    static idx_t capacity = 0;
    static idx_t next_k = 2;            /* next k to test */

    if (n == 0) return 0;               /* should not happen but safe */

    /* Grow cache when needed */
    if (capacity == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Find terms until we have at least n of them */
    while (count < n) {
        /* Compute k^2 - 3*k + 3 in 128‑bit to avoid overflow */
        unsigned __int128 sq = (unsigned __int128)next_k * next_k;
        unsigned __int128 val128 = sq - 3 * next_k + 3;
        uint64_t val = (uint64_t)val128;

        if (isPrime(val)) {
            if (count == capacity) {
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[count++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
