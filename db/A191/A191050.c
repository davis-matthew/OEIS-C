/*
 * A191050  Primes that are squares mod 83.
 * https://oeis.org/A191050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p is a quadratic * residue
 *            modulo 83 (i.e. Legendre symbol (p|83) = 1). * * The
 *            implementation keeps a dynamic cache of qualifying primes * and
 *            expands it on demand. Primes are generated incrementally * by
 *            trial division using the list of all primes found so far,...
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"

/* Static cache of qualifying primes (values of the sequence). */
static seq_t   *qual_primes   = NULL;
static size_t   qual_sz       = 0;
static size_t   qual_cap      = 0;

/* List of all primes discovered for trial division. */
static uint64_t *all_primes    = NULL;
static size_t   primes_sz      = 0;
static size_t   primes_cap     = 0;

/* Current candidate number being tested for primality. */
static uint64_t candidate       = 2;

/* Append a new qualifying prime to the cache.  The value is stored
 * as seq_t; any overflow is intentional for very small seq_t
 * types. */
static inline void append_qual(seq_t val)
{
    if (qual_sz == qual_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(qual_primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        qual_primes = tmp;
        qual_cap = new_cap;
    }
    qual_primes[qual_sz++] = val;
}

/* Append a newly found prime to the list used for trial division. */
static inline void append_prime(uint64_t p)
{
    if (primes_sz == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        all_primes = tmp;
        primes_cap = new_cap;
    }
    all_primes[primes_sz++] = p;
}

/* Ensure that the cache contains at least `need` qualifying primes.
 * The function is thread‑unsafe but callers are expected to be single
 * threaded. */
static void ensure_cache(idx_t need)
{
    while (qual_sz < (size_t)need) {
        bool is_prime = true;

        if (candidate == 2) {
            /* 2 is prime. */
        } else {
            /* Skip even numbers greater than 2. */
            if (candidate & 1ULL) {
                /* trial division only up to sqrt(candidate). */
                uint64_t limit = candidate;
                for (size_t i = 0; i < primes_sz; ++i) {
                    uint64_t p = all_primes[i];
                    if (p * p > candidate) break;
                    if (candidate % p == 0) {
                        is_prime = false;
                        break;
                    }
                }
            } else {
                is_prime = false;   /* even number > 2 is not prime */
            }
        }

        if (is_prime) {
            append_prime(candidate);

            /* 83 itself gives Jacobi symbol 0, skip it. */
            if (candidate != 83 && jacobi_symbol(candidate, 83) == 1) {
                /* Store as seq_t, overflowing if necessary. */
                append_qual((seq_t)candidate);
            }
        }

        /* Advance candidate: after 2 move to 3 and only test odds. */
        if (candidate <= 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }
}

/* External interface required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return qual_primes[n - 1];
}
