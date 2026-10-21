/*
 * A233181  Highly composite numbers of the form x^2 + xy + y^2.
 * https://oeis.org/A233181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   Sequence of highly composite numbers that can be written as
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* x^2 + x*y + y^2.  The terms are found by scanning all integers up to a */
 /* fixed bound, recording each new highly composite number that satisfies */
 /* the representation criterion. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

static seq_t *terms = NULL;    /* cache of sequence values */
static size_t terms_len  = 0;   /* number of cached terms       */
static size_t terms_cap  = 0;   /* allocated capacity           */
static bool   initialized = false;

/* Grow the terms array when needed. */
static void grow_terms(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!p) abort();           /* out‑of‑memory */
        terms   = p;
        terms_cap = new_cap;
    }
}

/* Build the list of terms once.  It scans all integers up to a limit. */
static void build_terms(void)
{
    /* 25200 is the largest known term; 1 000 000 is a safe upper bound. */
    const uint64_t MAX_LIMIT   = 1000000ULL;
    uint64_t max_divisors      = 0;   /* divisor count of the largest HCN seen */

    uint64_t primes[32], exps[32];
    size_t   cnt;

    for (uint64_t n = 1; n <= MAX_LIMIT; ++n) {
        seq_t dk  = divisor_count_u64(n);
        uint64_t d = (uint64_t)dk;

        if (d > max_divisors) {          /* n is a new highly composite number */
            max_divisors = d;

            /* Test whether n can be written as x^2 + xy + y^2. */
            factor_u64(n, primes, exps, &cnt);
            bool ok = true;
            for (size_t i = 0; i < cnt; ++i) {
                if ((primes[i] % 3U) == 2U && (exps[i] & 1U)) {
                    ok = false;           /* p ≡ 2 (mod 3) has odd exponent */
                    break;
                }
            }
            if (ok) {
                grow_terms();
                terms[terms_len++] = (seq_t)n;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) build_terms();

    if (n >= 1ULL && (size_t)n <= terms_len)
        return terms[n - 1];
    return (seq_t)0;  /* sequence is finite; 0 indicates beyond the last term */
}
