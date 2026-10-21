/*
 * A066650  Numbers not of the form a^2 + b^3 with a, b > 0.
 * https://oeis.org/A066650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers not of the form a^2 + b^3 with a,b>0 (OEIS A066650)
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* internal helper – generate terms up to a given count */
static idx_t last_limit = 0;            /* last value of the sieve limit */
static seq_t *terms = NULL;             /* array of discovered terms */
static size_t terms_len = 0;            /* number of terms stored */
static size_t terms_cap = 0;            /* capacity of the terms array */
static bool *present = NULL;            /* sieve bitmap: present[i]==true if i = a^2+b^3 */

/* ensure that at least target terms are available in 'terms' */
static void ensure_terms(idx_t target)
{
    if (terms_len >= target)
        return;

    /* start with a small limit, grow exponentially until we have enough terms */
    idx_t limit = last_limit ? last_limit * 2 : 1;
    if (limit < target * 2)
        limit = target * 2;   /* a rough lower bound */

    while (terms_len < target) {
        /* increase the sieve limit */
        limit *= 2;
        if (limit < target * 2)
            limit = target * 2;

        /* build sieve bitmap for [0..limit] */
        free(present);
        present = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (!present)               /* out of memory – abort */
            return;

        /* mark all numbers that can be written as a^2 + b^3 with a,b>0 */
        for (idx_t a = 1; (idx_t)a * a <= limit; ++a) {
            seq_t sq = (seq_t)a * a;
            for (idx_t b = 1; ; ++b) {
                seq_t cub = (seq_t)b * b * b;
                seq_t sum = sq + cub;
                if (sum > limit)
                    break;
                present[(size_t)sum] = true;
            }
        }

        /* append new non‑representable numbers to 'terms' */
        for (idx_t i = last_limit + 1; i <= limit; ++i) {
            if (!present[i]) {
                if (terms_len == terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) {          /* out of memory – abort */
                        free(present);
                        return;
                    }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)i;
            }
        }
        last_limit = limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is defined for n >= 1; guard anyway */
        return (seq_t)0;
    ensure_terms(n);
    return terms[n - 1];
}
