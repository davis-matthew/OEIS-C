/*
 * A065871  Numbers k such that usigma(k) + 1 is a prime.
 * https://oeis.org/A065871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 8539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer k such that sigma*(k)+1 is prime,
   where sigma* is the sum of the unitary divisors of k */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* cached terms of the sequence */
static seq_t *terms   = NULL;      /* array of terms */
static idx_t terms_len = 0;        /* number of terms cached */
static idx_t limit     = 200000;   /* current sieve limit */

/* recompute the term list up to the current limit */
static void recompute_terms(void)
{
    idx_t n = limit;

    /* linear sieve data */
    uint32_t *pr = NULL;   size_t pc = 0, cap = 0;
    uint32_t *spf = (uint32_t*)malloc((n+1) * sizeof(uint32_t));
    uint64_t *powp = (uint64_t*)malloc((n+1) * sizeof(uint64_t));
    seq_t *sigma = (seq_t*)malloc((n+1) * sizeof(seq_t));
    if (!spf || !powp || !sigma) exit(EXIT_FAILURE);

    /* initialise */
    spf[1] = 0;
    powp[1] = 1;
    sigma[1] = 1;
    for (idx_t i = 2; i <= n; ++i) spf[i] = 0;

    /* Build linear sieve and compute sigma* for every i */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            powp[i] = (uint64_t)i;
            sigma[i] = (seq_t)1 + (seq_t)i;
            if (pc == cap) {
                cap = cap?cap*2:16;
                pr = (uint32_t*)realloc(pr, cap * sizeof(uint32_t));
            }
            pr[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = pr[j];
            idx_t v = i * p;
            if (v > n) break;
            spf[v] = p;
            if (p == spf[i]) {
                /* same prime continues its maximal power */
                powp[v] = powp[i] * (uint64_t)p;
                sigma[v] = sigma[i / powp[i]] *
                           ((seq_t)1 + (seq_t)powp[v]);
            } else {
                powp[v] = p;
                sigma[v] = sigma[i] * ((seq_t)1 + (seq_t)p);
            }
        }
    }

    /* collect the terms */
    free(terms);
    size_t cap_terms = n / 10 + 100;
    terms = (seq_t*)malloc(cap_terms * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    terms_len = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (isPrime((idx_t)(sigma[i] + (seq_t)1))) {
            if (terms_len == cap_terms) {
                cap_terms *= 2;
                terms = (seq_t*)realloc(terms, cap_terms * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)i;
        }
    }

    /* free temporary arrays */
    free(spf); free(powp); free(sigma); free(pr);
}

/* ensure that at least n terms are cached */
static void ensure_terms(idx_t n)
{
    if (n <= terms_len) return;

    while (true) {
        recompute_terms();
        if (terms_len >= n) break;
        limit *= 2;                /* need more terms: increase limit */
    }
}

/* public entry point: a(n) returns the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_terms(n);
    return terms[n-1];
}
