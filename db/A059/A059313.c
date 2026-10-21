/*
 * A059313  Primes p such that x^25 = 2 has no solution mod p.
 * https://oeis.org/A059313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^25 = 2 has no solution mod p. */
/* The function enumerates primes and stores those where the congruence is unsolvable. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"      /* mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/next_prime.h"   /* next_prime(uint64_t) */

static bool has_solution(uint64_t p)
{
    if (p <= 2)
        return true;                         /* 0^25 ≡ 0 ≡ 2 (mod 2) */

    uint64_t pm1 = p - 1;
    uint64_t d;                              /* gcd(p-1,25) */
    if (pm1 % 25ULL == 0)
        d = 25;
    else if (pm1 % 5ULL == 0)
        d = 5;
    else
        d = 1;

    uint64_t exp = pm1 / d;
    seq_t val = mod_pow((seq_t)2, exp, (seq_t)p);
    return val == (seq_t)1;
}

static seq_t *cache = NULL;   /* dynamic array of collected primes */
static size_t len   = 0;      /* number of primes stored */
static size_t cap   = 0;      /* current capacity of cache   */
static uint64_t last_prime = 1;  /* last prime processed */

static void ensure_capacity(void)
{
    if (len >= cap)
    {
        size_t new_cap = (cap < 16) ? 16 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In a production environment we would handle malloc failure. */
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below offset is irrelevant */
        return 0;

    while (len < (size_t)n)
    {
        /* obtain the next prime after last_prime */
        last_prime = next_prime(last_prime + 1);

        if (!has_solution(last_prime))
        {
            ensure_capacity();
            cache[len++] = (seq_t)last_prime;
        }
    }
    return cache[(size_t)n - 1];
}
