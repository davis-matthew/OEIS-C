/*
 * A051086  Primes p such that x^34 = -2 has a solution mod p.
 * https://oeis.org/A051086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A051086: primes p such that the congruence x^34 ≡ –2 (mod p)
 * has a solution.  The function computes each term by testing integers
 * for primality and the congruence condition; 2 is included by hand.
 * The algorithm is linear in the number of primes examined.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *good_primes = NULL;   /* array of qualifying primes   */
static size_t good_size  = 0;       /* number of stored primes      */
static size_t good_cap   = 0;       /* current capacity             */
static idx_t  next_candidate = 2;   /* next integer to test         */

/* Append a new prime to the good_primes array, enlarging as needed. */
static void append_good(seq_t p)
{
    if (good_size == good_cap) {
        size_t nc = good_cap ? good_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(good_primes, nc * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        good_primes = tmp;
        good_cap = nc;
    }
    good_primes[good_size++] = p;
}

/* Test whether a prime p satisfies x^34 ≡ –2 (mod p). */
static bool good_prime(idx_t p)
{
    if (p == 2) return true;          /* 2 trivially works (x = 0) */

    /* Compute g = gcd(34, p – 1) */
    seq_t g = gcd((seq_t)34, (seq_t)(p - 1));

    /* Exponent for the Legendre symbol test: (p – 1) / g */
    seq_t exp_seq = ((seq_t)(p - 1)) / g;
    uint64_t exp = (uint64_t)exp_seq; /* fits for all primes needed */

    /* Compute (–2)^((p–1)/g) (mod p) */
    seq_t base = (seq_t)(p - 2);  /* –2 mod p, safe because p≥3 */
    seq_t mod  = (seq_t)p;
    seq_t r = mod_pow(base, exp, mod);

    return r == (seq_t)1;
}

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, but harmless */

    /* Generate primes until we have at least n qualifying ones. */
    while (good_size < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate)) ++next_candidate;

        if (good_prime(next_candidate))
            append_good((seq_t)next_candidate);

        ++next_candidate;
    }
    return good_primes[n - 1];
}
