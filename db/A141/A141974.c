/*
 * A141974  Primes congruent to 23 mod 28.
 * https://oeis.org/A141974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 23 (mod 28)
 * Range:     u8 n <= 5, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state held between calls */
static uint8_t *sieve = NULL;          /* 0 = composite, non‑zero = prime */
static size_t sieve_cap = 0;           /* current sieve limit */
static uint64_t *residue_primes = NULL;/* the primes ≡ 23 (mod 28) */
static size_t residue_len = 0;          /* number of stored residue primes */

/* --------------------------------------------------------------------- */
/*  Helper: grow the sieve to at least NEW_LIMIT                        */
/* --------------------------------------------------------------------- */
static void grow_sieve(size_t new_limit)
{
    /* free previous sieve */
    if (sieve) {
        free(sieve);
        sieve = NULL;
    }
    /* allocate new sieve */
    sieve = (uint8_t *)calloc(new_limit + 1, sizeof(uint8_t));
    if (!sieve) abort();
    sieve_cap = new_limit;

    /* classic Eratosthenes */
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (!sieve[p]) {
            for (size_t k = p * p; k <= new_limit; k += p) {
                sieve[k] = 1; /* composite */
            }
        }
    }

    /* build list of residues 23 (mod 28) */
    free(residue_primes);
    residue_primes = NULL;
    residue_len = 0;
    size_t est_cap = new_limit / 28 + 10;
    residue_primes = (uint64_t *)malloc(est_cap * sizeof(uint64_t));
    if (!residue_primes) abort();
    for (size_t i = 23; i <= new_limit; i += 28) {
        if (!sieve[i]) { /* prime */
            if (residue_len == est_cap) {
                est_cap *= 2;
                residue_primes = (uint64_t *)realloc(residue_primes,
                                                     est_cap * sizeof(uint64_t));
                if (!residue_primes) abort();
            }
            residue_primes[residue_len++] = i;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Ensure we have at least N residue primes cached                       */
/* --------------------------------------------------------------------- */
static void ensure_n(idx_t N)
{
    if (N == 0) return;           /* no term for 0 */
    if (residue_len >= (size_t)N) return;

    /* start with a reasonable limit and double until we have enough */
    size_t limit = 200;            /* initial guess */
    if (sieve_cap) {
        limit = sieve_cap;         /* reuse previous limit as starting point */
    }

    while (residue_len < (size_t)N) {
        limit *= 2;                /* enlarge */
        grow_sieve(limit);
    }
}

/* --------------------------------------------------------------------- */
/*  Public API: returns the nth term (n ≥ 1) of the sequence             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for 0, but harmless */
    ensure_n(n);
    return (seq_t)residue_primes[n - 1];
}
