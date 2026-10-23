/*
 * A109374  Irregular table read by rows: Row n is the terms of the continued
 *          fraction for prime(n+1)/prime(n).
 * https://oeis.org/A109374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) is the n-th term obtained by listing the simple continued
 *            fraction terms of prime(n+1)/prime(n) in row‑wise order
 * Range:     u8 n <= 346, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t); */

static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;   /* number of terms already computed */

static uint64_t *primes   = NULL;
static size_t primes_cap  = 0;
static size_t primes_cnt  = 0;  /* number of primes stored */

/* Append a new prime value to the primes array */
static void add_prime(uint64_t val)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = (primes_cap) ? (primes_cap * 2) : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = val;
}

/* Append the next prime after the largest stored one */
static void add_next_prime(void)
{
    uint64_t cand = (primes_cnt) ? (primes[primes_cnt-1] + 1) : 2;
    while (!is_prime(cand)) ++cand;
    add_prime(cand);
}

/* Append a term to the global terms array */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap) ? (terms_cap * 2) : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence offset starts at 1 */

    /* Ensure at least two primes are available for the first row */
    if (primes_cnt == 0) add_prime(2);
    if (primes_cnt == 1) add_prime(3);

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Current row uses primes[primes_cnt-1] / primes[primes_cnt-2] */
        uint64_t num = primes[primes_cnt-1];
        uint64_t den = primes[primes_cnt-2];

        /* Euclidean algorithm to produce continued‑fraction terms */
        while (den != 0) {
            uint64_t q = num / den;
            uint64_t r = num % den;
            push_term((seq_t)q);
            num = den;
            den = r;
        }

        /* Prepare next prime for the following row */
        add_next_prime();
    }

    return terms[n-1];   /* array is 0‑indexed, sequence is 1‑indexed */
}
