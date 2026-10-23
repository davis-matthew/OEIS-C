/*
 * A191070  Primes p that have Kronecker symbol (p|39) = -1.
 * https://oeis.org/A191070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function that returns the p‑th prime p for which (p|39) = –1
   using a memoised sieve of primes up to the required bound. */

/* Primes p with Kronecker symbol (p|39) = –1 are exactly those
   primes for which LegendreSymbol(p,3) * LegendreSymbol(p,13) = –1.
   (3 and 13 are the prime factors of 39; 3 and 13 themselves are excluded.)
   The test is performed directly from the residue classes modulo 3 and 13. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple trial‑division primality test for 64‑bit integers.
   It is fast enough for the first 10 000 terms (the 10 000‑th term is < 10⁶). */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)       return false;
    return true;
}

/* Test whether a prime p satisfies (p|39) = –1. */
static inline bool good_prime(uint64_t p)
{
    /* Exclude the primes dividing 39. */
    if (p == 3 || p == 13)    return false;

    /* Legendre symbol (p|3) : +1 if p ≡ 1 (mod 3), –1 if p ≡ 2 (mod 3). */
    int leg3 = (p % 3 == 1) ? 1 : -1;

    /* Legendre symbol (p|13). */
    static const int residues13[6] = {1,3,4,9,10,12};
    int leg13 = -1;                     /* default –1 */
    uint64_t r = p % 13;
    for (int i = 0; i < 6; ++i)
        if (r == (uint64_t)residues13[i]) { leg13 = 1; break; }

    return (leg3 * leg13) == -1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of found primes (stored as uint64_t for safety in
       arbitrary‑precision builds: we convert to seq_t on return). */
    static uint64_t *prime_cache = NULL; /* array of primes that satisfy the test */
    static size_t   sz          = 0;      /* number of elements stored */
    static size_t   cap         = 0;      /* allocated capacity */
    static uint64_t curr        = 2;      /* next odd number to test for prime */

    /* Ensure enough capacity for the next element. */
    if (sz + 1 > cap) {
        size_t newcap = (cap == 0) ? 8 : cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, newcap * sizeof(uint64_t));
        if (!tmp) abort();          /* allocation failure */
        prime_cache = tmp;
        cap = newcap;
    }

    /* Extend the cache until it contains at least n elements. */
    while (sz < (size_t)n) {
        if (is_prime_uint64(curr) && good_prime(curr))
            prime_cache[sz++] = curr;
        /* Next candidate: after 2 only odd numbers are needed. */
        if (curr == 2) curr = 3;
        else           curr += 2;
    }

    /* Convert the stored uint64_t prime to seq_t on return. */
    return (seq_t)prime_cache[n - 1];
}
