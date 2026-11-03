/*
 * A187563  Least k >=0 and k < prime(n) such that prime(n)*(prime(n)-k)-1 and
 *          prime(n)*(prime(n)-k)+1 are twin primes or -1 if no such k exists.
 * https://oeis.org/A187563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 34, i16 n <= 25000, i32 n <= 25000, i64 n <= 25000,
 *            i128 n <= 25000, big any n
 * Validated: standard, pass, 75 terms through n = 9612 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): least k ≥ 0 and k < prime(n) such that
 * prime(n)*(prime(n)-k) ± 1 are twin primes.
 * Return -1 if no such k exists. */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------
 * 16‑bit or 32‑bit builds: seq_t may not hold numbers up to
 * prime(n)^2.  We therefore perform primality tests on
 * unsigned 64‑bit integers and return the result as seq_t
 * (k is always < prime(n) and fits in 32 bits).
 * ------------------------------------------------------------ */

/* Deterministic Miller–Rabin for uint64_t (primes < 2^64).
   Bases  {2,325,9375,28178,450775,9780504,1795265022} are enough. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n < 4)                return true;   /* 2,3 */
    if ((n & 1ULL) == 0ULL)   return false;  /* even */

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (int i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, p = a, dtmp = d;
        /* modular exponentiation: x = a^d mod n */
        while (dtmp > 0) {
            if (dtmp & 1ULL) x = (x * p) % n;
            p = (p * p) % n;
            dtmp >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n - 1) continue;

        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x64 = (__uint128_t)x64 * x64 % n;
            if (x64 == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------
 *  Utility: get_prime(idx_t k) returns the k-th prime (1‑based).
 *  Utilities are assumed to be provided by the framework.
 * ------------------------------------------------------------ */
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* ------------------------------------------------------------
 *  Main sequence function required by the framework.
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n must be ≥ 1.  If not, return -1. */
    if (n == 0) return (seq_t)-1;

    /* Prime(n) as 32‑bit unsigned integer. */
    uint32_t p = get_prime(n);

    /* Special case: p = 2 -> k = 0, always works. */
    if (p == 2U) return (seq_t)0;

    /* Search odd k from 1 upwards; k < p. */
    for (uint32_t k = 1; k < p; k += 2U) {
        /* m = p * (p - k) fits in 64 bits (p ≤ 1e6 for n up to 10000). */
        uint64_t pk = (uint64_t)p - (uint64_t)k;
        uint64_t m  = (uint64_t)p * pk;

        /* Check twin primes m-1 and m+1. */
        uint64_t a1 = m - 1;
        uint64_t a2 = m + 1;
        if (is_prime_uint64(a1) && is_prime_uint64(a2))
            return (seq_t)k;
    }

    /* No suitable k found */
    return (seq_t)-1;
}
