/*
 * A173132  Numbers k with the property that both the sum of the even digits
 *          of k^2 and the sum of the odd digits of k^2 are squares.
 * https://oeis.org/A173132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 843, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173132: numbers k such that both the sum of the even digits
 * of k² and the sum of the odd digits of k² are perfect squares.
 * The function computes this by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* Squares up to 400 – all digit sums of k² that occur for the range we
 * consider are covered by this table. */
static inline bool is_square_small(unsigned long long v) {
    switch (v) {
        case 0: case 1: case 4: case 9: case 16: case 25:
        case 36: case 49: case 64: case 81: case 100: case 121:
        case 144: case 169: case 196: case 225: case 256: case 289:
        case 324: case 361: case 400: return true;
        default: return false;
    }
}

seq_t a(idx_t n) {
    /* static cache of generated terms */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static unsigned long long last_k = 0;   /* last k examined */

    /* initialise on first call */
    if (!terms) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms[0] = (seq_t)0;
        terms_cnt = 1;
        last_k = 0;
    }

    /* generate further terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        unsigned long long k = last_k + 1;
        last_k = k;

        unsigned long long k2 = k * k;
        unsigned long long sum_even = 0, sum_odd = 0;

        if (k2 == 0) {
            /* sums stay 0 */
        } else {
            while (k2 > 0) {
                unsigned int d = (unsigned int)(k2 % 10);
                if ((d & 1) == 0)
                    sum_even += d;
                else
                    sum_odd += d;
                k2 /= 10;
            }
        }

        if (is_square_small(sum_even) && is_square_small(sum_odd)) {
            /* store this term */
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1; /* double capacity */
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)k;
        }
    }

    return terms[(size_t)(n - 1)];
}
