/*
 * A141469  Nonprimes of the form c(c(i)-3)-3, where A002808(i)=c(i)=i-th
 *          composite.
 * https://oeis.org/A141469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Nonprimes of the form c(c(n)‑3)‑3, where c(k) is the k‑th
 *            composite number (starting at 4)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Static storage for composites and the sequence. */
static seq_t *composites = NULL;
static size_t composites_count = 0;
static size_t composites_cap    = 0;
static idx_t last_comp_candidate = 4;          /* next number to test for composite */

/* Builds composites[0..need‑1] (1‑based c(i)) */
static void ensure_composites(idx_t need)
{
    while (composites_count < need) {
        seq_t cand = (seq_t)last_comp_candidate;
        if (!is_prime_small(cand)) {        /* composite */
            if (composites_count == composites_cap) {
                composites_cap = composites_cap ? composites_cap * 2 : 16;
                composites = (seq_t *)realloc(composites,
                                             composites_cap * sizeof(seq_t));
            }
            composites[composites_count++] = cand;
        }
        last_comp_candidate++;
    }
}

/* Storage for the computed sequence A141469. */
static seq_t *seq   = NULL;
static size_t seq_count = 0;
static size_t seq_cap    = 0;
static idx_t  next_i     = 1;                 /* next i to process */

/* Append a value to the sequence storage. */
static void append_seq(seq_t val)
{
    if (seq_count == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_count++] = val;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, but harmless */

    /* Generate sequence until we have at least n terms. */
    while (seq_count < n) {
        idx_t i = next_i;                 /* current index i */

        /* Ensure we have c(i). */
        ensure_composites(i);
        seq_t ci = composites[i - 1];     /* c(i) */

        /* Compute inner index = c(i) – 3. */
        idx_t idx2 = (idx_t)(ci - 3);

        /* Ensure we have c(c(i) – 3). */
        ensure_composites(idx2);
        seq_t cci = composites[idx2 - 1];

        /* Value to test: c(c(i) – 3) – 3. */
        seq_t val = cci - 3;

        /* Record if nonprime (composite, 1, or 0). */
        if (!is_prime_small(val)) {
            append_seq(val);
        }
        next_i++;                          /* move to next i */
    }

    return seq[n - 1];
}
