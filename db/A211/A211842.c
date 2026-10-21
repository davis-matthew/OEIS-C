/*
 * A211842  Number of nonnegative integer arrays of length n+3 with new values
 *          0 upwards introduced in order, no three adjacent elements all
 *          unequal, and containing the value 2.
 * https://oeis.org/A211842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length (n+3) restricted growth arrays * with no
 *            3 consecutive distinct values, containing the value 2 * This
 *            implementation uses a linear–time DP that keeps track of the *
 *            last two elements being equal or distinct and the current number
 *            of * distinct values. The answer for a give...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* The maximum length we need to support.  For n up to about 10000
 * the length n+3 is less than 20000, which is safely inside this
 * bound. */
#define MAX_LEN 20000

/* DP arrays.  They are indexed by distinct‑value count d (1 <= d <= len).
 * eq[d]  – sequences of current length whose last two elements are equal
 * neq[d] – sequences of current length whose last two elements are
 *          distinct
 */
static seq_t eq[MAX_LEN];
static seq_t neq[MAX_LEN];
static seq_t eq_next[MAX_LEN];
static seq_t neq_next[MAX_LEN];
/* sum_by_len[l] – number of sequences of length l that contain the value 2.
 * For l < 4 this is 0. */
static seq_t sum_by_len[MAX_LEN];
/* How far we have already computed the DP (in terms of sequence length). */
static size_t computed_len = 1;    /* we have not computed anything yet */

/* Update the DP tables until the specified target_len is reached. */
static void ensure_len(size_t target_len) {
    if (computed_len >= target_len) return;

    /* Initialise the base case for length 2 */
    if (computed_len == 1) {
        memset(eq, 0, sizeof(eq));
        memset(neq, 0, sizeof(neq));
        eq[1] = 1;   /* sequence [0,0] */
        neq[2] = 1;  /* sequence [0,1] */
        sum_by_len[1] = 0;
        sum_by_len[2] = 0;
        computed_len = 2;
    }

    /* Extend the DP step by step */
    for (size_t len = computed_len; len < target_len; ++len) {
        size_t next_len = len + 1;

        /* Clear temporary tables up to the next length */
        memset(eq_next, 0, sizeof(seq_t) * (next_len + 1));
        memset(neq_next, 0, sizeof(seq_t) * (next_len + 1));

        for (size_t d = 1; d <= len; ++d) {
            seq_t val_eq  = eq[d];
            seq_t val_neq = neq[d];

            if (val_eq) {
                /* Pick the existing value equal to the repeated one */
                eq_next[d] += val_eq;

                /* Pick one of the other existing values (d-1 choices) */
                if (d > 1)
                    neq_next[d] += val_eq * (seq_t)(d - 1);

                /* Introduce a new value (only allowed when last two are equal) */
                neq_next[d + 1] += val_eq;
            }

            if (val_neq) {
                /* From a distinct pair, one child stays distinct, the other becomes equal */
                neq_next[d] += val_neq;   /* choose the earlier element -> distinct pair */
                eq_next[d]  += val_neq;   /* choose the later element -> equal pair */
            }
        }

        /* Copy the results back into the current tables */
        for (size_t d = 1; d <= next_len; ++d) {
            eq[d] = eq_next[d];
            neq[d] = neq_next[d];
        }

        /* Compute the answer for the new length: sequences with at least 3 distinct values */
        seq_t total = 0;
        for (size_t d = 3; d <= next_len; ++d) {
            total += eq[d];
            total += neq[d];
        }
        sum_by_len[next_len] = total;
    }

    computed_len = target_len;
}

/* ----------------------------------------------------------------------
 * Main function required by the build.
 * ----------------------------------------------------------------------
 */
seq_t a(idx_t n) {
    /* The array length is n + 3.  For n == 1 the length is 4, etc. */
    size_t len = (size_t)n + 3;

    /* Ensure we've precomputed DP up to this length. */
    ensure_len(len);

    return sum_by_len[len];
}
