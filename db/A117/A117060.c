/*
 * A117060  Mersenne numbers for which the product of the digits is not zero.
 * https://oeis.org/A117060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 29, u128 n <= 35,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117060: Mersenne numbers 2^k−1 whose decimal representation contains no zero digit.
 * Computed by incrementally generating 2^k and retaining those with no zero digit. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* static cache of terms already found */
    static seq_t *terms = NULL;      /* array of Mersenne numbers found */
    static size_t term_len = 0;     /* number of terms currently stored */
    static size_t term_cap = 0;     /* capacity of the array */

    /* state for generating the next candidate */
    static idx_t cur_exp = 1;      /* exponent k corresponding to cur_val */
    static seq_t cur_val = (seq_t)2; /* cur_val = 2^k, starting with k=1 */

    /* initialise the cache on first call */
    if (!terms) {
        term_cap = 32;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        term_len = 0;
    }

    /* generate terms until we have at least n of them */
    while (term_len < (size_t)n) {
        /* count decimal digits of cur_val */
        unsigned int freq[10];
        digit_frequencies(cur_val, freq);
        if (freq[0] == 0) {   /* no zero digit */
            /* store 2^k – 1 */
            seq_t val = cur_val - (seq_t)1;
            if (term_len == term_cap) {
                /* grow the cache */
                size_t new_cap = term_cap * 2;
                seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
                for (size_t i = 0; i < term_len; ++i)
                    new_terms[i] = terms[i];
                free(terms);
                terms = new_terms;
                term_cap = new_cap;
            }
            terms[term_len++] = val;
        }

        /* advance to the next power of two (2^(k+1)) */
#if defined(__cplusplus)
        cur_val = cur_val * (seq_t)2;
#else
        cur_val = (seq_t)(cur_val * 2);
#endif
        ++cur_exp;
    }

    return terms[n - 1];
}
