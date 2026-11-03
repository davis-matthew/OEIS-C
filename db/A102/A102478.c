/*
 * A102478  Numbers n such that the concatenations (2*n),(2*n-1) and
 *          (2*n),(2*n+1) give twin primes.
 * https://oeis.org/A102478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 396, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Validated: standard, pass, 65 terms through n = 6705 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A102478: numbers n such that the decimal concatenations
   (2*n , 2*n-1) and (2*n , 2*n+1) form a pair of twin primes.  The
   function returns the n-th such number (1‑based).  It uses a memoised
   sequential search and a deterministic Miller‑Rabin primality test
   for 64‑bit integers via is_prime(uint64_t). */

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

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

static int digits(idx_t n) {
    int d = 1;
    while (n >= 10) {
        n /= 10;
        d++;
    }
    return d;
}

static seq_t *cache = NULL;      /* stored terms of the sequence   */
static idx_t cache_size = 0;     /* number of terms already stored */
static idx_t next_candidate = 1; /* next n to test */

/* Helper: test whether n satisfies the twin‑prime concatenation
   property.  All arithmetic is performed in 64‑bit integers, which
   is sufficient for the first ~10^6 candidates. */
static bool good_candidate(idx_t cand) {
    uint64_t two = 2ull * cand;                    /* 2*n          */
    int d1 = digits((idx_t)(two - 1));              /* digits of 2*n-1 */
    uint64_t pow10d1 = (uint64_t)pow10(d1);          /* 10^d1        */
    uint64_t p1 = two * pow10d1 + (two - 1);        /* concat 2n(2n-1) */
    if (!is_prime(p1)) return false;

    int d2 = digits((idx_t)(two + 1));              /* digits of 2*n+1 */
    uint64_t pow10d2 = (uint64_t)pow10(d2);
    uint64_t p2 = two * pow10d2 + (two + 1);        /* concat 2n(2n+1) */
    if (!is_prime(p2)) return false;

    return true; /* both are prime, hence twins */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for zero index */
    /* extend the cache until it contains the requested term */
    while (cache_size < n) {
        if (good_candidate(next_candidate)) {
            /* grow the array by one element */
            seq_t *tmp = mem_resize(seq_t, cache, (cache_size + 1));
            if (!tmp) return 0;     /* allocation failed – abort */
            cache = tmp;
            cache[cache_size] = (seq_t)next_candidate; /* store the n */
            cache_size++;
        }
        next_candidate++;            /* next integer to test */
    }
    return cache[n - 1];
}
