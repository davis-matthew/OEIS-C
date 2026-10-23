/*
 * A115687  Powerful(1) numbers (A001694) whose digit reversal is a semiprime
 *          (A001358).
 * https://oeis.org/A115687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A115687 – Powerful(1) numbers whose digit reversal is a
 *            semiprime (Generator by brute force search with memoisation)
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   factor_u64, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/rev_decimal.h"

static seq_t* terms = NULL;      /* sorted list of terms found so far */
static size_t  terms_len  = 0;   /* number of terms currently stored */
static size_t  terms_cap  = 0;   /* capacity of the terms array */

/* Checks whether a 64‑bit integer is powerful (every prime factor exponent ≥ 2) */
static int is_powerful_uint64(uint64_t n)
{
    if (n < 4) return 0;   /* 1,2,3 are not powerful */
    uint64_t pr[16], ex[16];
    size_t   cnt;
    factor_u64(n, pr, ex, &cnt);
    if (cnt == 0) return 0;            /* n == 1 */
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] < 2) return 0;
    return 1;
}

/* Checks whether a 64‑bit integer is a semiprime (exactly two prime factors counted with multiplicity) */
static int is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return 0;   /* 2 and 3 are not semiprime */
    uint64_t pr[16], ex[16];
    size_t   cnt;
    factor_u64(n, pr, ex, &cnt);
    if (cnt == 0) return 0;
    unsigned sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += (unsigned)ex[i];
    return sum == 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise storage for terms */
    if (terms == NULL) {
        terms_cap = 64;
        terms     = (seq_t*)malloc(sizeof(seq_t) * terms_cap);
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        uint64_t cand = 0;
        if (terms_len == 0) {
            cand = 1;               /* start from 1 (will skip it) */
        } else {
            cand = (uint64_t)terms[terms_len - 1] + 1;
        }

        for ( ; ; ++cand) {
            if (!is_powerful_uint64(cand)) continue;

            /* reverse decimal digits */
            seq_t rev_seq = rev_decimal((seq_t)cand);
            uint64_t rev = (uint64_t)rev_seq;   /* fits easily for the required n */

            if (is_semiprime_uint64(rev)) {
                /* store new term */
                if (terms_len == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t*)realloc(terms, sizeof(seq_t) * terms_cap);
                }
                terms[terms_len++] = (seq_t)cand;
                break;   /* proceed to next candidate for subsequent terms */
            }
        }
    }

    return terms[n - 1];
}
