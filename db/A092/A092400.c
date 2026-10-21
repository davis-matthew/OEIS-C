/*
 * A092400  Fixed point of the morphism 1 -> 1121211, 2 -> 1121212121211,
 *          starting from a(1) = 1.
 * https://oeis.org/A092400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092400: fixed point of the morphism  
 *  1 → 1121211 , 2 → 1121212121211 , starting from a(1)=1.  
 *  The sequence is obtained by repeatedly applying the morphism to
 *  the current word until at least n symbols have been produced. */

#include <stdlib.h>

/* Remember the fixed point as we grow it.  The buffer is expanded only
 * when a new level of the morphism is applied.  The array elements are
 * always 1 or 2. */
static seq_t *seq = NULL;
static idx_t  seq_len = 0;

/* Return the n-th term of A092400.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, unused case */

    /* Initialise the sequence with the first symbol (1). */
    if (seq == NULL) {
        seq_len = 1;
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = 1;
    }

    /* Grow the sequence until it contains at least n symbols. */
    while (seq_len < n) {
        /* Compute the length of the next level. */
        idx_t new_len = 0;
        for (idx_t i = 0; i < seq_len; ++i)
            new_len += (seq[i] == 1) ? 7 : 13;   /* 1 → 7, 2 → 13 symbols */

        /* Build the next level in a freshly allocated buffer. */
        seq_t *new_seq = (seq_t *)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t v = seq[i];
            if (v == 1) {
                /* 1 → 1121211 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
            } else { /* v == 2 */
                /* 2 → 1121212121211 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
            }
        }

        /* Replace the old sequence with the new one. */
        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    /* Return the requested term (1‑based indexing). */
    return seq[n - 1];
}
