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
 * Helpers:   ensure_spf, is_prime64, is_semiprime, sieve_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime64.h"
#include "../utils/ensure_spf.h"
#include "../utils/is_semiprime.h"
#include "../utils/sieve_spf.h"

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
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
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
        if (is_prime64((uint64_t)(next_k - 2))) {
            /* semiprime check */
            if (is_semiprime((uint64_t)next_k, spf, spf_limit)) {
                /* store term */
                if (terms_cnt == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
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
