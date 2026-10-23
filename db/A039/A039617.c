/*
 * A039617  Solution to a "self-describing sequence" puzzle of Raphael
 *          Robinson.
 * https://oeis.org/A039617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the finite self‑describing sequence A039617.
   The sequence consists of pairs (c_d , d) where c_d is the total
   number of decimal occurrences of digit d in the full sequence.
   The algorithm finds the fixed point of the mapping that
   recalculates the counts from the sequence until convergence. */

/* Include the utility that counts decimal digits of a seq_t value. */
#include "../utils/digit_counts.h"
#include <stdbool.h>

static seq_t counts[10] = {0};   /* caches the fixed point counts */
static bool init_done = false;

/* Compute the fixed point counts for the self‑describing sequence.   */
static void compute_counts(void)
{
    seq_t oldc[10] = {0};
    seq_t newc[10];
    const unsigned max_iter = 100;
    unsigned iter = 0;

    /* iterate until the vector of counts stabilizes. */
    while (1) {
        /* zero new counts */
        for (unsigned i = 0; i < 10; ++i) newc[i] = 0;

        /* build the sequence using current guesses in oldc */
        for (unsigned d = 0; d < 10; ++d) {
            /* count digits of the term representing the count itself */
            unsigned char digit_cnt[10];
            digit_counts(oldc[d], digit_cnt);
            for (unsigned k = 0; k < 10; ++k) newc[k] += digit_cnt[k];

            /* count the digit d itself (each pair ends with d) */
            newc[d] += 1;
        }

        /* check for convergence */
        bool same = true;
        for (unsigned i = 0; i < 10; ++i) {
            if (newc[i] != oldc[i]) { same = false; break; }
        }
        if (same) break;

        /* prepare for next iteration */
        for (unsigned i = 0; i < 10; ++i) oldc[i] = newc[i];
        if (++iter >= max_iter) break;  /* safety guard, should not trigger */
    }

    /* copy the converged counts into the global cache */
    for (unsigned i = 0; i < 10; ++i) counts[i] = oldc[i];
}

/* Return the n-th term of the sequence (n is zero‑based).  */
seq_t a(idx_t n)
{
    /* initialize the cache on first use */
    if (!init_done) {
        compute_counts();
        init_done = true;
    }

    /* The sequence has 20 terms: pairs (c_d,d) for d = 0..9.  */
    if (n < 20) {
        if ((n & 1) == 0) {          /* even index – count term */
            return counts[n >> 1];
        } else {                     /* odd index – digit term */
            return (seq_t)(n >> 1);
        }
    }

    /* For indices beyond the defined sequence, return 0.  */
    return (seq_t)0;
}
