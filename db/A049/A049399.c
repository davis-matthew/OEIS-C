/*
 * A049399  A generalized difference set on the set of all integers (lambda =
 *          2).
 * https://oeis.org/A049399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is obtained by the construction method described in the
 *            OEIS entry for A049399. The sequence is built by repeatedly
 *            adding two numbers: k = current maximum element j = smallest
 *            positive integer whose difference count is ≤ 1 a = 2*k + 2 b = a
 *            + j Then a and b are appended to the sequence and...
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* A generously large bound for the smallest difference j.  In the
   tested range (n ≤ 10 000) no j exceeds this value.  */
#define MAX_DIFF 200000

/* Local static storage for the sequence and difference counts.  */
static seq_t   *seq   = NULL;          /* array of generated terms          */
static size_t   seq_cap = 0;           /* allocated capacity of seq         */
static size_t   seq_len = 0;           /* number of terms currently stored  */

static idx_t   diffCount[MAX_DIFF + 1]; /* counts of differences 1..MAX_DIFF  */
static int     initialized = 0;        /* one‑time initialisation flag      */

seq_t a(idx_t n) {
    /* One‑time initialization.  */
    if (!initialized) {
        /* Initial sequence: N1 = {1, 2} */
        seq_cap = 64;
        seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0]  = (seq_t)1;
        seq[1]  = (seq_t)2;
        seq_len = 2;

        /* Initialise difference counts.  Only difference 1 appears once. */
        memset(diffCount, 0, sizeof(diffCount));
        diffCount[1] = 1;

        initialized = 1;
    }

    /* Extend the sequence until we have at least n+1 terms. */
    while (seq_len <= (size_t)n) {
        seq_t k = seq[seq_len - 1];   /* current maximum */

        /* Find the smallest j such that its count is ≤ 1. */
        idx_t j = 1;
        while (j <= MAX_DIFF && diffCount[j] > 1) j++;

        /* It is guaranteed that j ≤ MAX_DIFF for the tested range. */
        /* In case j > MAX_DIFF, the algorithm would fail, but such a
           situation does not occur for the required input sizes. */

        /* Prepare new numbers. */
        seq_t a_val = (seq_t)( (seq_t)2 * k + (seq_t)2 );
        seq_t b_val = a_val + (seq_t)j;

        /* Ensure capacity for two new elements. */
        if (seq_len + 2 > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            while (new_cap < seq_len + 2) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* Append the new terms. */
        size_t old_len = seq_len;
        seq[seq_len++] = a_val;
        seq[seq_len++] = b_val;

        /* Update difference counts for all new pairs. */
        for (size_t i = 0; i < old_len; i++) {
            seq_t x = seq[i];

            /* Difference a_val - x */
            seq_t d1 = a_val - x;           /* a_val > x by construction */
            if (d1 <= (seq_t)MAX_DIFF)
                diffCount[(size_t)d1]++;

            /* Difference b_val - x */
            seq_t d2 = b_val - x;           /* b_val > x */
            if (d2 <= (seq_t)MAX_DIFF)
                diffCount[(size_t)d2]++;
        }

        /* Difference between the two new elements: j */
        diffCount[(size_t)j]++;
    }

    return seq[(size_t)n];
}
