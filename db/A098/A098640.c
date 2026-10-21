/*
 * A098640  a(n) = 2^p + 1 where p is the n-th prime.
 * https://oeis.org/A098640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(p) + 1 where p is the n‑th prime (a(1) = 5)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Include the arbitrary‑precision exponentiation routine. */
#include "../utils/big_pow.h"

/* --------------------------------------------------------------------- */
/* Prime storage and generation                                         */
/* --------------------------------------------------------------------- */
#define MAX_PRIMES 110000          /* sufficient for the first 10000 primes */

static idx_t primes[MAX_PRIMES];   /* primes[0] = 2, primes[1] = 3, … */
static size_t primes_count = 0;    /* number of primes stored              */
static idx_t next_candidate = 3;   /* next odd number to test               */
static bool initialized = false;   /* whether primes[0] has been set       */

/* Ensure that the first `need` primes are available. */
static void ensure_primes(idx_t need)
{
    if (need == 0) return;

    if (!initialized) {
        primes[0] = 2;
        primes_count = 1;
        next_candidate = 3;
        initialized = true;
    }

    while (primes_count < (size_t)need) {
        idx_t cand = next_candidate;
        next_candidate += 2;                /* only odd candidates */

        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            idx_t p = primes[i];
            if (p * p > cand) break;        /* sqrt test */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_count < (size_t)MAX_PRIMES)
                primes[primes_count++] = cand;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Sequence function                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  For n == 0 return 0. */
    if (n == 0) return (seq_t)0;

    /* Get the n‑th prime (indexing from 0). */
    idx_t prime_idx = n - 1;               /* zero‑based */
    ensure_primes(prime_idx + 1);

    idx_t p = primes[prime_idx];

    /* Compute 2^p + 1 using big_pow (arbitrary precision). */
    seq_t two = (seq_t)2;
    seq_t power = big_pow(two, (seq_t)p);
    return power + (seq_t)1;
}
