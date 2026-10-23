/*
 * A100657  Composite numbers whose prime factors all have the same digital
 *          root.
 * https://oeis.org/A100657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100657: Composite numbers whose prime factors all have the same digital root. 
   Implementation: linear search with trial division and dynamic arrays. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array for seq_t values.  Works in C and C++ (realloc cast added for C++). */
static seq_t *terms      = NULL;
static size_t terms_size = 0, terms_cap = 0;
static seq_t current_checked = 3;   /* last number examined */

/* Prime list for trial division. */
static seq_t *primes      = NULL;
static size_t prime_count = 0, prime_cap = 0;
static seq_t sieve_limit  = 0;       /* largest integer for which primes are known */

/* Helper: add a prime number to the primes array. */
static void add_prime(seq_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        prime_cap = new_cap;
    }
    primes[prime_count++] = p;
}

/* Test whether x is prime using currently known primes. */
static bool is_prime_candidate(seq_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that all primes up to 'limit' are stored. */
static void ensure_primes_up_to(seq_t limit)
{
    if (limit <= sieve_limit) return;
    for (seq_t cand = sieve_limit + 1; cand <= limit; ++cand) {
        if (is_prime_candidate(cand)) add_prime(cand);
    }
    sieve_limit = limit;
}

/* Add a found term to the terms array. */
static void add_term(seq_t v)
{
    if (terms_size == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_size++] = v;
}

/* Check if m is composite and all prime factors have same digital root. */
static bool matches(seq_t m)
{
    if (m <= 3) return false;

    /* Get the first prime for trial division to seed prime list. */
    if (prime_count == 0) add_prime(2);

    seq_t temp = m;
    bool has_factor = false;
    uint64_t dr = 0, root = 0;
    bool root_set = false;
    bool ok = true;

    /* Factorization by trial division using known primes. */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            has_factor = true;
            uint64_t d = (uint64_t)(p % 9);
            if (d == 0) d = 9;
            if (!root_set) {
                root = d; root_set = true;
            } else if (root != d) {
                ok = false; break;
            }
            while (temp % p == 0) temp /= p;
        }
    }
    if (!ok) return false;

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        uint64_t d = (uint64_t)(temp % 9);
        if (d == 0) d = 9;
        if (!root_set) {
            root = d; root_set = true;
        } else if (root != d) {
            return false;
        }
    }

    /* Must be composite (at least one proper factor). */
    if (!has_factor) return false;
    return true;
}

/* Return the nth term of A100657. */
seq_t a(idx_t n)
{
    if (n <= terms_size) return terms[n - 1];

    seq_t cand = current_checked + 1;
    for ( ; terms_size < (size_t)n ; ++cand) {
        /* Ensure primes up to at least sqrt(cand). */
        ensure_primes_up_to(cand);

        if (matches(cand)) add_term(cand);
        current_checked = cand;
    }
    return terms[n - 1];
}
