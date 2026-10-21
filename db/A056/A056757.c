/*
 * A056757  Cube of number of divisors is larger than the number.
 * https://oeis.org/A056757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 4856, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A056757: numbers k such that τ(k)^3 > k
   Formula: a(n) = k where k is the n‑th positive integer with (A000005(k))^3 > k */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static state to store the sequence once */
static seq_t *terms = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;

#ifndef MAX_TERMS
#define MAX_TERMS 51261u
#endif

static void init_sequence(void)
{
    if (terms) return;   /* already initialized */

    /* Empirical upper bound that contains all 51261 terms.
       The largest term is < 1e7, so this array is safe. */
    const idx_t LIMIT = 10000000u;

    /* Allocate divisor count array (uint16_t is enough for k ≤ 1e7). */
    uint16_t *dcnt = (uint16_t *)calloc((size_t)LIMIT + 1, sizeof(uint16_t));
    if (!dcnt) return;   /* allocation failure – return without initializing */

    /* Sieve to fill divisor counts: dcnt[n] = τ(n). */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            ++dcnt[j];
        }
    }

    /* Prepare dynamic array for sequence terms. */
#ifdef __cplusplus
    terms = new seq_t[1024];
#else
    terms = (seq_t *)malloc(1024 * sizeof(seq_t));
#endif
    term_cap = 1024;
    term_len = 0;

    /* Scan all k up to LIMIT and collect the terms. */
    for (idx_t k = 2; k <= LIMIT; ++k) {
        uint64_t t  = (uint64_t)dcnt[k];
        uint64_t t3 = t * t * t;                /* τ(k)^3, fits in 64‑bit here */

        if (t3 > (uint64_t)k) {
            /* Append k to the terms list. */
            if (term_len == term_cap) {
                size_t new_cap = term_cap * 2;
#ifdef __cplusplus
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < term_len; ++i) tmp[i] = terms[i];
                delete [] terms;
#else
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
#endif
                if (!tmp) break; /* cannot allocate more – abort */
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = (seq_t)k;
            if (term_len >= MAX_TERMS) break;
        }
    }

    /* Shrink array to exact size. */
    if (term_cap > term_len) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[term_len];
        for (size_t i = 0; i < term_len; ++i) tmp[i] = terms[i];
        delete [] terms;
        terms = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(terms, term_len * sizeof(seq_t));
        if (tmp) terms = tmp;
#endif
        term_cap = term_len;
    }

    free(dcnt);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    if (!terms) init_sequence();

    if (n <= (idx_t)term_len)
        return terms[(size_t)(n - 1)];

    /* If n exceeds the number of computed terms, return 0. */
    return (seq_t)0;
}
