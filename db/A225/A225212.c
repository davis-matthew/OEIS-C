/*
 * A225212  A "Look and Say" sequence (with nested repetitions).
 * https://oeis.org/A225212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A225212 is obtained by concatenating blocks that are the *
 *            run‑length encoding of all previous terms, starting with block1
 *            = [1]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the sequence */
static seq_t *seq  = NULL; /* array of terms */
static idx_t seq_len = 0;  /* number of used elements */
static idx_t seq_cap = 0;  /* allocated capacity */

/* Ensure the array has room for at least 'needed' elements. */
static void ensure_cap(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n-th term of A225212 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 */
        return (seq_t)0;

    /* First call: initialise with block1. */
    if (seq == NULL) {
        ensure_cap(n * 2 + 10);     /* ample space for first block */
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Generate successive whole blocks until we have at least n terms. */
    while (seq_len < n) {
        idx_t old_len = seq_len;     /* block is defined only by the
                                       sequence *before* this block */
        idx_t i = 0;
        while (i < old_len) {
            idx_t j = i + 1;
            seq_t val = seq[i];
            while (j < old_len && seq[j] == val)
                j++;

            idx_t run = j - i;       /* length of the run */

            /* Append count and value of the run. */
            ensure_cap(seq_len + 2);
            seq[seq_len]     = (seq_t)run;
            seq[seq_len + 1] = val;
            seq_len += 2;

            i = j;
        }
    }

    return seq[n - 1];
}
