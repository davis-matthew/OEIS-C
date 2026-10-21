/*
 * A059668  Primes p such that x^36 = 2 has no solution mod p, but x^12 = 2
 *          has a solution mod p.
 * https://oeis.org/A059668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 78, u32 n <= 357, u64 n <= 357, u128 n <= 357,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059668: Primes p such that 2 is a 12th-power residue mod p
    but not a 36th‑power residue.
    a(n) = the n-th prime p for which
           ∃ x: x^12 ≡ 2 (mod p)   and   ∀ y: y^36 ≠ 2 (mod p).   n≥1
    (Offset 1 – a(1)=919).  The implementation tests each prime
    in turn and keeps the qualifying ones in a static cache.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* idx_t and seq_t are supplied by the build environment.  */

static uint64_t *good_primes = NULL;   /* cache of qualifying primes */
static size_t    good_len    = 0;      /* number of cached primes      */
static size_t    good_cap    = 0;      /* allocated capacity           */

/* Prime generation ---------------------------------------------------- */
static uint64_t *primes = NULL;   /* all primes found so far      */
static size_t    plen   = 0;       /* number of primes             */
static size_t    pcap   = 0;       /* capacity of primes array     */
static uint64_t  candidate = 3;   /* next odd candidate to test   */

static void prune_prime_cap(size_t need) {
    if (pcap >= need) return;
    size_t new_cap = pcap ? pcap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    pcap = new_cap;
}

static void add_prime(uint64_t p) {
    prune_prime_cap(plen + 1);
    primes[plen++] = p;
}

static uint64_t next_prime(void) {
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < plen; ++i) {
            uint64_t q = primes[i];
            __uint128_t q2 = (__uint128_t)q * q;
            if (q2 > candidate) break;
            if (candidate % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            add_prime(candidate);
            candidate += 2;
            return primes[plen - 1];
        }
        candidate += 2;
    }
}

/* --------------------------------------------------------------------
   Modulo exponentiation for 64‑bit modulus and exponent up to 64 bits.
   Returns 2^exp mod mod.  The result always fits in uint64_t.
   -------------------------------------------------------------------- */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* --------------------------------------------------------------------
   Ensure that at least `need` qualifying primes are cached.
   -------------------------------------------------------------------- */
static void ensure_good_primes(size_t need) {
    while (good_len < need) {
        uint64_t p = next_prime();   /* get next prime > 2 */
        if (p == 2) continue;

        uint64_t G  = p - 1;
        uint64_t g12 = G % 12 == 0 ? 12 : G;          /* gcd(12,G) */
        /* Compute gcd of 12 and G explicitly */
        uint64_t a = G, b = 12;
        while (b) { uint64_t t = b; b = a % b; a = t; } 
        g12 = a;
        uint64_t e12 = G / g12;

        /* gcd(36,G) */
        a = G; b = 36;
        while (b) { uint64_t t = b; b = a % b; a = t; }
        uint64_t g36 = a;
        uint64_t e36 = G / g36;

        /* Check conditions */
        if (modpow_uint64(2, e12, p) != 1ULL) continue;
        if (modpow_uint64(2, e36, p) == 1ULL)  continue;

        /* Store the prime */
        if (good_len == good_cap) {
            good_cap = good_cap ? good_cap * 2 : 64;
            good_primes = (uint64_t *)realloc(good_primes, good_cap * sizeof(uint64_t));
        }
        good_primes[good_len++] = p;
    }
}

/* --------------------------------------------------------------------
   The function requested by the platform.
   -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure_good_primes((size_t)n);
    return (seq_t)good_primes[n - 1];
}
