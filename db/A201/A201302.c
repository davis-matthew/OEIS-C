/*
 * A201302  Primes of the form 9n^3+8.
 * https://oeis.org/A201302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 84, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 44 terms through n = 3919 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime of the form 9k³+8, k≥1 (k odd only) */
/* Searches for primes of the form 9k³+8 using a deterministic 64‑bit
   Miller‑Rabin test provided by ../utils/is_prime.h.  The routine
   memoises found primes so that multiple calls are efficient. */

/* It assumes the following includes are available: */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

seq_t a(idx_t n)
{
    /* Lazy initialisation of static data */
    static seq_t *primes = NULL;
    static size_t size = 0;          /* number of primes stored */
    static size_t capacity = 0;      /* allocated capacity */
    static idx_t last_k = 1;         /* last k tested (odd) */
    static bool initialised = false;

    if (!initialised) {
        capacity = 1024;
        primes   = mem_new(seq_t, capacity);
        size = 0;
        last_k = 1;
        initialised = true;
    }

    /* Grow the prime storage when necessary */
    while (size < (size_t)n) {
        /* Compute next candidate: 9*k^3+8, with k odd */
        __int128 tmp = (__int128)last_k * last_k * last_k; /* k³ */
        seq_t p = (seq_t)(9 * tmp + 8);                     /* 9k³+8 */

        /* Primitive 64‑bit primality test (k³ < 2¹⁶⁺… ensures p < 2⁶⁴) */
        if (is_prime((uint64_t)p)) {
            if (size == capacity) {          /* grow array */
                capacity *= 2;
                primes = mem_resize(seq_t, primes, capacity);
            }
            primes[size++] = p;               /* store prime */
        }

        last_k += 2;                          /* only odd k produce odd p */
    }

    return primes[n - 1];
}
