/*
 * A019369  Primes with primitive root 44.
 * https://oeis.org/A019369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that 44 is a primitive root modulo p (A019369) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* --------------------------------------------------------------------- */
/*  Static state for caching the primes found                                 */
/* --------------------------------------------------------------------- */
static seq_t *prime_cache    = NULL;   /* array of found primes      */
static size_t prime_cache_len = 0;     /* number of stored primes   */
static size_t prime_cache_cap = 0;     /* allocated capacity        */
static uint64_t last_checked = 1;      /* last integer examined     */

/* --------------------------------------------------------------------- */
/*  Append a prime to the cache, reallocating as necessary                */
/* --------------------------------------------------------------------- */
static void append_cache(uint64_t p)
{
    if (prime_cache_len == prime_cache_cap) {
        size_t new_cap = (prime_cache_cap == 0) ? 8 : prime_cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* unrecoverable allocation error */
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_len++] = (seq_t)p;
}

/* --------------------------------------------------------------------- */
/*  Test whether 44 is a primitive root modulo the prime p               */
/* --------------------------------------------------------------------- */
static bool has_primitive_root_44(uint64_t p)
{
    /* 44 must be coprime with p; exclude 2 and 11 */
    if (p == 2 || p == 11) return false;

    uint64_t phi = p - 1;
    uint64_t temp = phi;

    /* Collect distinct prime factors of phi */
    uint64_t primes[64];
    size_t pc = 0;

    /* Trial division to factor phi's distinct primes */
    for (uint64_t d = 2; d <= temp / d; d = (d == 2) ? 3 : d + 2) {
        if (temp % d == 0) {
            primes[pc++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1)
        primes[pc++] = temp;

    /* Check the primitive‑root condition */
    for (size_t i = 0; i < pc; ++i) {
        uint64_t q = primes[i];
        seq_t r = mod_pow((seq_t)44, (idx_t)(phi / q), (seq_t)p);
        if (r == (seq_t)1) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Public function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safe placeholder for out‑of‑range */

    while (prime_cache_len < (size_t)n) {
        /* Find the next prime */
        do {
            ++last_checked;
        } while (!is_prime((idx_t)last_checked));

        /* Store it if 44 is a primitive root modulo it */
        if (has_primitive_root_44(last_checked))
            append_cache(last_checked);
    }

    return prime_cache[(size_t)n - 1];
}
