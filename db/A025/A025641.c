/*
 * A025641  Exponent of 3 (value of i) in n-th number of form 3^i*6^j.
 * https://oeis.org/A025641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is obtained by enumerating all 3‑smooth numbers of the form
 *            2^a*3^b with b ≥ a, ordering the numbers increasingly and
 *            outputting (b−a). This yields the OEIS sequence A025641
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Single entry representing exponent pair (a,b) of 2^a*3^b. */
typedef struct {
    uint32_t a;    /* exponent of 2   */
    uint32_t b;    /* exponent of 3   */
} exp_pair_t;

/* *********************************************************************
   The required function: returns the nth term of sequence A025641.
   ********************************************************************* */
seq_t a(idx_t n)
{
    /* n is at least 1 for the sequence; for safety handle n==0. */
    if (n == 0) {
        return 0;
    }

    /* Allocate initial buffer for exponent pairs. */
    size_t capacity = 1024;
    exp_pair_t *seq = (exp_pair_t *)malloc(capacity * sizeof(exp_pair_t));
    if (!seq) {
        /* Allocation failure – return 0 as a harmless fallback. */
        return 0;
    }

    /* The first 3‑smooth number is 2^0*3^0 = 1. */
    seq[0].a = 0;
    seq[0].b = 0;
    size_t sz = 1;               /* number of generated pairs */

    /* Indices used by the standard two‑pointer algorithm. */
    size_t i2 = 0;               /* index for multiplying by 2 (a+1) */
    size_t i3 = 0;               /* index for multiplying by 3 (b+1) */

    /* Count of pairs that satisfy b ≥ a (the filtered sequence). */
    idx_t filtered = 1;          /* the first pair already satisfies it */
    seq_t answer   = 0;          /* exponent difference for the nth filtered pair */

    /* Use long double for accurate comparison of 2^a*3^b values. */
    const long double ln2 = logl(2.0L);
    const long double ln3 = logl(3.0L);
    const long double EPS = 1e-15L;   /* tolerance for equality */

    while (filtered < n) {
        /* Candidate by multiplying current i2 element by 2: (a+1, b). */
        uint32_t cand_a1 = seq[i2].a + 1;
        uint32_t cand_b1 = seq[i2].b;
        long double log1 = (long double)cand_a1 * ln2 + (long double)cand_b1 * ln3;

        /* Candidate by multiplying current i3 element by 3: (a, b+1). */
        uint32_t cand_a2 = seq[i3].a;
        uint32_t cand_b2 = seq[i3].b + 1;
        long double log2 = (long double)cand_a2 * ln2 + (long double)cand_b2 * ln3;

        /* Select the smaller numeric value (or both if equal). */
        uint32_t new_a, new_b;
        if (log1 < log2 - EPS) {
            new_a = cand_a1;
            new_b = cand_b1;
            ++i2;
        } else if (log2 < log1 - EPS) {
            new_a = cand_a2;
            new_b = cand_b2;
            ++i3;
        } else {
            /* The two candidates represent the same number. */
            new_a = cand_a1;
            new_b = cand_b1;
            ++i2;
            ++i3;
        }

        /* Resize the array if necessary. */
        if (sz >= capacity) {
            capacity <<= 1;
            exp_pair_t *tmp = (exp_pair_t *)realloc(seq, capacity * sizeof(exp_pair_t));
            if (!tmp) {
                free(seq);
                return answer;
            }
            seq = tmp;
        }

        /* Store the new pair. */
        seq[sz].a = new_a;
        seq[sz].b = new_b;
        ++sz;

        /* If this pair lies in the filtered subset, update counters. */
        if (new_b >= new_a) {
            ++filtered;
            answer = (seq_t)(new_b - new_a);   /* b−a */
        }
    }

    free(seq);
    return answer;
}
