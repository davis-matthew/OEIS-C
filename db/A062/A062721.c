/*
 * A062721  Numbers k such that k is a product of two primes and k-2 is prime.
 * https://oeis.org/A062721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the numbers k such that * k is a product of two primes
 *            (counted with multiplicity) and * k‑2 is prime. They are listed
 *            in increasing order, * a(1)=4, a(2)=9, …
 * Range:     u8 n <= 24, u16 n <= 2132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Helper: simple sieve to fill spf[2..limit] (smallest prime factor) */
static void sieve_spf(uint32_t *spf, size_t limit) {
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (size_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint32_t)i;
}

/* Grow spf array if needed to include 'need'. */
static void ensure_spf(uint32_t **spf, size_t *limit, uint64_t need) {
    if (need <= *limit) return;
    size_t new_limit = *limit;
    while (new_limit < need) new_limit = new_limit * 2 + 1;
    uint32_t *new_spf = (uint32_t*)realloc(*spf, (new_limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();
    *spf = new_spf;
    if (new_limit > *limit) {
        /* Initialize new entries to zero before sieving */
        memset((*spf) + *limit + 1, 0, (new_limit - *limit) * sizeof(uint32_t));
        *limit = new_limit;
        sieve_spf(*spf, *limit);
    }
}

/* Primitive is_semiprime: true iff x has exactly two prime factors (with mult.). */
static bool is_semiprime(uint64_t x, uint32_t *spf, size_t spf_limit) {
    if (x < 4) return false;
    if (x > spf_limit) return false;   /* cannot decide */
    int cnt = 0;
    uint64_t tmp = x;
    while (tmp > 1 && cnt < 2) {
        uint32_t f = spf[(size_t)tmp];
        ++cnt;
        tmp /= f;
    }
    return (tmp == 1) && (cnt == 2);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for 0; return 0 */
    /* Static state */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0, terms_cap = 0;
    static uint32_t *spf = NULL;
    static size_t spf_limit = 0;
    static uint64_t next_k = 4;           /* start checking from 4 */

    /* Initialize static structures on first call */
    if (!terms) {
        terms_cap = 1024;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) abort();
        terms_cnt = 0;
    }
    if (!spf) {
        spf_limit = 1048576;     /* 1<<20, enough for first growth */
        spf = (uint32_t*)calloc(spf_limit + 1, sizeof(uint32_t));
        if (!spf) abort();
        sieve_spf(spf, spf_limit);
    }

    /* Grow terms array if necessary */
    while (terms_cnt < n) {
        /* Ensure spf covers next_k */
        ensure_spf(&spf, &spf_limit, next_k);

        /* k-2 primality check */
        if (is_prime_u64((uint64_t)(next_k - 2))) {
            /* semiprime check */
            if (is_semiprime((uint64_t)next_k, spf, spf_limit)) {
                /* store term */
                if (terms_cnt == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                    if (!tmp) abort();
                    terms = tmp;
                }
                terms[terms_cnt++] = next_k;
            }
        }
        ++next_k;
    }
    return terms[n - 1];
}
