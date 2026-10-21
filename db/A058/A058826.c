/*
 * A058826  Even cototient numbers which are nontotients.
 * https://oeis.org/A058826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the OEIS sequence A058826: */
/*  a(n) = the n-th even cototient that is not a totient. */
/*  The sequence is produced by a sieve that generates all totients
 *  and cototients up to a fixed limit (5 000 000).  All numbers
 *  that are even, occur as a cototient, and are not totients
 *  are collected into a list.  The function then returns the
 *  requested index from that list.  The limit is large enough
 *  to contain at least the first 10 000 terms. */

#include <stdint.h>
#include <stdlib.h>

static const size_t LIMIT = 5000000UL;   /* sieve range */

/* One-time storage for the computed terms. */
static seq_t *terms = NULL;
static size_t term_count = 0;
static size_t term_cap = 0;

/* Helper arrays for the sieve. */
static uint64_t *phi = NULL;
static uint8_t *is_comp = NULL;
static uint8_t *is_totient = NULL;
static uint8_t *is_cototient = NULL;

/* Build all terms up to LIMIT.  This is done only once. */
static void build_terms(void)
{
    /* Allocate sieve arrays.  Cast explicitly for C++ compatibility. */
    phi          = (uint64_t*)malloc((LIMIT + 1) * sizeof(uint64_t));
    is_comp      = (uint8_t*)malloc((LIMIT + 1) * sizeof(uint8_t));
    is_totient   = (uint8_t*)calloc((LIMIT + 1), sizeof(uint8_t));
    is_cototient = (uint8_t*)calloc((LIMIT + 1), sizeof(uint8_t));

    /* Linear sieve for Euler’s totient. */
    uint32_t *primes = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t prime_cnt = 0;

    phi[1] = 1;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            phi[i] = (uint64_t)(i - 1);
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            size_t v = i * (size_t)p;
            if (v > LIMIT) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                phi[v] = phi[i] * (uint64_t)p;
                break;
            } else {
                phi[v] = phi[i] * (uint64_t)(p - 1);
            }
        }
    }

    /* Mark totients. */
    for (size_t i = 1; i <= LIMIT; ++i) {
        uint64_t t = phi[i];
        if (t <= LIMIT) is_totient[(size_t)t] = 1;
    }

    /* Mark cototients. */
    for (size_t i = 2; i <= LIMIT; ++i) {
        uint64_t cot = (uint64_t)i - phi[i];
        if (cot <= LIMIT) is_cototient[(size_t)cot] = 1;
    }

    /* Collect qualifying numbers into the terms list. */
    term_cap = 2048;
    terms = (seq_t*)malloc(term_cap * sizeof(seq_t));
    term_count = 0;

    for (size_t n = 2; n <= LIMIT; ++n) {
        if ((n & 1ULL) == 0 &&
            !is_totient[n] &&
            is_cototient[n]) {
            if (term_count == term_cap) {
                term_cap <<= 1;
                terms = (seq_t*)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)n;   /* cast to seq_t */
        }
    }

    /* Free temporary arrays. */
    free(phi);
    free(is_comp);
    free(is_totient);
    free(is_cototient);
    free(primes);
}

/* Function required by the framework. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        build_terms();
        initialized = 1;
    }

    /* n is 1-based; guard for out-of-range indices. */
    if (n == 0 || (size_t)n > term_count) return (seq_t)0;

    return terms[(size_t)n - 1];
}
