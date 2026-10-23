/*
 * A023236  Primes p such that 9*p + 10 is also prime.
 * https://oeis.org/A023236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 9 p + 10 is also prime.
   Computed by incremental search with trial–division primality test. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Helper data structures ---------- */
static uint64_t *prime_list = NULL;      /* primes found so far */
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;

static seq_t *term_cache   = NULL;      /* cached sequence terms */
static size_t   term_cap     = 0;
static size_t   term_len     = 0;

/* ---------- Helper functions ---------- */

/* Reallocate the prime list to at least new_cap elements. */
static void ensure_prime_cap(size_t new_cap)
{
    if (prime_cap >= new_cap) return;
    if (prime_cap == 0) prime_cap = 16;
    while (prime_cap < new_cap) prime_cap <<= 1;
    prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
}

/* Append a new prime to the prime list. */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* Test whether n (>=2) is prime using trial division by primes already found. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Reallocate the sequence cache to at least new_cap elements. */
static void ensure_term_cap(size_t new_cap)
{
    if (term_cap >= new_cap) return;
    if (term_cap == 0) term_cap = 16;
    while (term_cap < new_cap) term_cap <<= 1;
    term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
}

/* Append a new term to the sequence cache. */
static void add_term(uint64_t p)
{
    if (term_len < term_cap) term_cache[term_len++] = (seq_t)p;
    else {
        ensure_term_cap(term_len + 1);
        term_cache[term_len++] = (seq_t)p;
    }
}

/* Generate sequence terms up to at least the desired index n_target. */
static void generate_up_to(size_t n_target)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 2;
    if (prime_cnt == 0) candidate = 2;

    while (term_len < n_target) {
        /* Find the next prime candidate. */
        if (candidate < 2) candidate = 2;
        while (!is_prime_uint64(candidate)) {
            candidate += (candidate == 2 ? 1 : 2); /* after 2 only odd numbers */
        }
        /* candidate is a prime */
        add_prime(candidate);

        /* Check the property 9*p + 10 is prime */
        uint64_t test = 9ULL * candidate + 10ULL;
        if (is_prime_uint64(test)) {
            add_term(candidate);
        }

        /* Advance candidate to the next integer to test */
        candidate += (candidate == 2 ? 1 : 2);
    }
}

/* ---------- The required exported function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Undefined for n < 1, but return 0 */
    generate_up_to((size_t)n);
    return term_cache[(size_t)n - 1];
}
