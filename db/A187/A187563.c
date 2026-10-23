/*
 * A187563  Least k >=0 and k < prime(n) such that prime(n)*(prime(n)-k)-1 and
 *          prime(n)*(prime(n)-k)+1 are twin primes or -1 if no such k exists.
 * https://oeis.org/A187563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 34, i16 n <= 9612, i32 n <= 9612, i64 n <= 9612,
 *            i128 n <= 9612, big any n
 * Helpers:   get_prime
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
#include "../utils/get_prime.h"

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
