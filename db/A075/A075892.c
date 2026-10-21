/*
 * A075892  Average of squares of successive primes: a(n) = (prime(n+1)^2 +
 *          prime(n)^2)/2, with n >= 2.
 * https://oeis.org/A075892
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+1)^2 + prime(n)^2)/2 (offset 2)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A075892.  The function keeps a dynamically expanding
 * array of the first consecutive primes and uses trial division for
 * prime generation.  For each call it guarantees that at least n+1 primes
 * are available and then computes the average of their squares, cast to
 * the returned type seq_t.  The calculation uses a 128‑bit intermediate
 * to avoid overflow during the square calculations.  */
 
#include <stdlib.h>
#include <stdint.h>
 
/* ------------------------------------------------------------------ */
/* dynamic prime cache                                                */
static uint64_t *prime_cache = NULL;   /* holds the found primes */
static size_t   prime_count = 0;       /* number of primes stored   */
static size_t   prime_cap   = 0;       /* capacity of prime_cache   */
 
/* ------------------------------------------------------------------ */
/* trial division prime test using the cached primes */
static int is_prime_candidate(uint64_t candidate)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return 0;
    }
    return 1;
}
 
/* ------------------------------------------------------------------ */
/* ensure that at least 'need' primes are cached */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;
 
    /* initialise with the first prime if the cache is empty */
    if (prime_count == 0) {
        prime_cap   = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cache[0] = 2;
        prime_count   = 1;
    }
 
    uint64_t candidate = prime_cache[prime_count - 1] + 1;
    while (prime_count < need) {
        if (candidate > 2 && (candidate & 1) == 0) { /* skip even numbers */
            ++candidate;
            continue;
        }
        if (is_prime_candidate(candidate)) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_count++] = candidate;
        }
        ++candidate;
    }
}
 
/* ------------------------------------------------------------------ */
/* compute the sequence value for the given index (offset 2)           */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2, return 0 */
 
    /* ensure we have primes for indices n-1 and n (1‑based) */
    ensure_primes((size_t)n + 1);
 
    uint64_t p1 = prime_cache[n - 1];     /* prime(n)   */
    uint64_t p2 = prime_cache[n];         /* prime(n+1) */
 
    /* use 128‑bit arithmetic for the squares, then divide by 2 */
    unsigned __int128 sum128 = (unsigned __int128)p1 * p1 +
                               (unsigned __int128)p2 * p2;
    return (seq_t)(sum128 / 2);
}
