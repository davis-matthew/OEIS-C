/*
 * A191250  Fixed point of the morphism 0 -> 01, 1 -> 02, 2 -> 001.
 * https://oeis.org/A191250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191250: a(n) is the n‑th symbol of the fixed point of the
   morphism 0 → 01, 1 → 02, 2 → 001. The function builds the sequence
   incrementally up to the requested index. */
#include <stdlib.h>

static seq_t *seq = NULL;      /* cached sequence values */
static idx_t  seq_len = 0;     /* number of values currently stored */

/* Ensure that seq contains at least n terms (1‑based). */
static void expand_to(idx_t n)
{
    if (seq_len >= n)
        return;

    /* initialise the seed sequence 0 if this is the first call */
    if (seq == NULL) {
        seq_len = 1;
        seq = (seq_t*)malloc(sizeof(seq_t));
        seq[0] = 0;
    }

    /* repeatedly apply the morphism until we have enough terms */
    while (seq_len < n) {
        /* compute length of the next iteration */
        idx_t next_len = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t v = seq[i];
            if (v == 0 || v == 1)
                next_len += 2;
            else            /* v == 2 */
                next_len += 3;
        }

        /* build the next iteration */
        seq_t *next = (seq_t*)malloc(next_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t v = seq[i];
            if (v == 0) {          /* 0 → 0 1 */
                next[pos++] = 0;
                next[pos++] = 1;
            }
            else if (v == 1) {     /* 1 → 0 2 */
                next[pos++] = 0;
                next[pos++] = 2;
            }
            else {                 /* 2 → 0 0 1 */
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
            }
        }

        free(seq);
        seq = next;
        seq_len = next_len;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n=0, safe default */

    expand_to(n);
    return seq[n-1];
}
