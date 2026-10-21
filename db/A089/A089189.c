/*
 * A089189  Primes p such that p-1 is cubefree.
 * https://oeis.org/A089189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 4584, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p such that p‑1 is cubefree
   (OEIS A089189).  It builds a sieve of smallest prime factors and
   scans primes incrementally, caching the sequence terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve utilities ------------------------------------ */
static uint64_t current_limit = 0;
static uint32_t *spf = NULL;           /* smallest prime factor table */
static uint64_t *prime_list = NULL;    /* list of primes */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static size_t prime_idx = 0;           /* index of next prime to examine */

/* Reallocate or allocate a new spf table up to 'limit'. */
static void init_sieve(uint64_t limit)
{
    if (limit <= current_limit) return;   /* already sufficient */

    /* Allocate / reallocate spf array */
    if (spf) free(spf);
    spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) abort();                    /* out of memory */

    /* Simple Eratosthenes sieve for smallest prime factor */
    for (uint64_t i = 2; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (uint64_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* Build the prime list from the spf table */
    if (prime_list) free(prime_list);
    prime_cap = (size_t)(limit / 2 + 10);
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) abort();
    prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
        }
    }
    current_limit = limit;
}

/* Test whether x is cubefree using the precomputed spf table. */
static bool is_cubefree(uint64_t x)
{
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) {
            x /= p;
            if (++e >= 3) return false;      /* exponent ≥ 3 found */
        }
    }
    return true;
}

/* ---------- Cached sequence storage ---------------------------------- */
static seq_t *terms = NULL;          /* cached sequence terms */
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a new term to the dynamic array of sequence values. */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? (terms_cap << 1) : 8;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* Ensure initial sieve size sufficient for starting work */
    if (current_limit == 0) init_sieve(200000);

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* If we have exhausted current primes, extend the sieve */
        if (prime_idx >= prime_cnt) {
            uint64_t new_limit = current_limit * 2;
            init_sieve(new_limit);
            /* prime_idx stays where it was – primes before this index
               are already examined, only new primes will follow.     */
        }

        /* Examine next prime */
        uint64_t p = prime_list[prime_idx++];
        if (is_cubefree(p - 1)) {
            add_term((seq_t)p);
        }
    }

    return terms[n - 1];
}
