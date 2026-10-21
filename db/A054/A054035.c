/*
 * A054035  Numbers n such that n^2 contains exactly 7 different digits.
 * https://oeis.org/A054035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that n^2 contains exactly 7 different digits
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   distinct_digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for such numbers and memoises the sequence. */

#include <stdlib.h>
#include "../utils/distinct_digit_count.h"

static seq_t *terms = NULL;           /* array of found terms (1‑based from offset)  */
static size_t  terms_alloc = 0;      /* allocated length of terms array            */
static idx_t   terms_count = 0;      /* number of terms stored so far             */
static idx_t   next_candidate = 1;   /* next integer to test                      */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but safe */

    /* Initialise storage on first call. */
    if (terms_count == 0) {
        terms_alloc = 128;
        terms = (seq_t*)malloc(terms_alloc * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (terms_count < n) {
        /* Find the next integer whose square has exactly 7 distinct digits. */
        for (;;) {
            seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;
            if (distinct_digit_count(sq) == 7) {
                /* Store the found term. */
                if (terms_count >= terms_alloc) {
                    terms_alloc = (terms_alloc ? terms_alloc << 1 : 128);
                    terms = (seq_t*)realloc(terms, terms_alloc * sizeof(seq_t));
                }
                terms[terms_count++] = next_candidate;
                ++next_candidate;   /* prepare for next search */
                break;
            }
            ++next_candidate;
        }
    }

    return terms[n - 1];
}
