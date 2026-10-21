/*
 * A051272  Number of numbers neither relatively prime to nor divisors of
 *          primorial number(n).
 * https://oeis.org/A051272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n primes * − product of (prime − 1)
 *            for the same n * − 2^n + 1 * This follows from a(n) = P_n −
 *            prod(p–1) − 2^n + 1, where * P_n is the nth primorial
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow2_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2_n.h"

static size_t  primes_cap = 0;
static size_t  primes_cnt = 0;
static uint64_t *primes = NULL;    /* array of the first primes */
static seq_t   prodP = (seq_t)1;   /* product of primes seen so far */
static seq_t   prodQ = (seq_t)1;   /* product of (prime−1) seen so far */

/* append a new prime to the list, update products */
static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                       /* failed to allocate */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
    prodP *= (seq_t)p;
    prodQ *= (seq_t)(p - 1ULL);
}

/* simple prime test by trial division using the primes we already know */
static int is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* make sure we have stored at least 'need' primes */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need) return;

    /* start candidate from last known prime + 1, or 2 if none */
    uint64_t cand = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;
    while (primes_cnt < need) {
        if (is_prime_candidate(cand)) add_prime(cand);
        ++cand;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1, but guard against 0 */

    ensure_primes((size_t)n);

    /* product of (prime‐1) and product of primes have been built up to n */
    seq_t two_n = pow2_n(n);
    seq_t result = prodP - prodQ - two_n + (seq_t)1;
    return result;
}
