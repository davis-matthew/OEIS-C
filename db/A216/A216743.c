/*
 * A216743  Primes p such that x^24 = -2 has no solution mod p.
 * https://oeis.org/A216743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^24 = -2 has no solution mod p. */
/* This function builds a list of such primes on demand and returns the n‑th
   one (1‑based).  It tests each prime p>3 using the criterion
   (-2)^((p-1)/gcd(24,p-1)) ≠ 1 (mod p).  The implementation uses a
   simple trial‑division primality test for the primes it scans.  The
   list is cached across calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* ---- small helper: gcd for uint64_t ---------------------------------- */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---- small helper: trial division primality test --------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if ((n & 1) == 0)   return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* ---- dynamic array handling ------------------------------------------- */
static void *grow_array(void *ptr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap ? *cap * 2 : 8);
        if (new_cap < need) new_cap = need;
        ptr = realloc(ptr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
    return ptr;
}

/* ---- test whether -2 is a 24th power residue modulo p ---------------- */
static bool no_solution(uint64_t p)
{
    /* p > 3 in usage */
    uint64_t d = gcd_uint64(24ULL, p - 1ULL);
    uint64_t exp = (p - 1ULL) / d;
    uint64_t a = p - 2ULL;                       /* -2 mod p, p>2 */
    seq_t res = mod_pow((seq_t)a, exp, (seq_t)p);
    return res != (seq_t)1;
}

/* ---- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;          /* primes that fail the congruence */
    static size_t   cap   = 0;
    static size_t   count = 0;
    static uint64_t last  = 4ULL;             /* primes are >3 */

    /* Extend list until it contains at least n elements. */
    while (count < n) {
        /* find next prime > last */
        uint64_t p = last + 1;
        while (!is_prime_uint64(p)) ++p;

        /* test congruence */
        if (no_solution(p)) {
            primes = (uint64_t *)grow_array(primes, &cap, count + 1);
            primes[count++] = p;
        }
        last = p;
    }

    return (seq_t)primes[n - 1];
}
