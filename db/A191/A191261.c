/*
 * A191261  Fixed point of the morphism 0 -> 01, 1 -> 002, 2 -> 01.
 * https://oeis.org/A191261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: expand the fixed point of the morphism
   0 → 01, 1 → 002, 2 → 01 until the resulting word
   contains the requested position.  a(n) is the nth symbol
   (1‑based) of that infinite word. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of the word up to the largest index seen */
    static seq_t *seq = NULL;   /* array of symbols */
    static size_t seq_len = 0;  /* current number of stored symbols */

    /* initial word: '0' */
    if (seq == NULL) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq_len = 1;
        seq[0] = (seq_t)0;
    }

    /* grow the sequence until it contains position n */
    while (seq_len < (size_t)n) {
        size_t old_len = seq_len;
        size_t new_len = 0;

        /* first pass: determine the length of the next word */
        for (size_t i = 0; i < old_len; ++i) {
            seq_t v = seq[i];
            if (v == (seq_t)0 || v == (seq_t)2)
                new_len += 2;         /* 0 → 01 , 2 → 01 */
            else /* v == 1 */
                new_len += 3;         /* 1 → 002 */
        }

        /* allocate space for the expanded word */
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;

        /* second pass: perform the expansion */
        for (size_t i = 0; i < old_len; ++i) {
            seq_t v = seq[i];
            if (v == (seq_t)0 || v == (seq_t)2) {
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
            } else { /* v == 1 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)2;
            }
        }

        free(seq);
        seq = next;
        seq_len = new_len;
    }

    /* return the requested symbol (1‑based index) */
    return seq[(size_t)(n - 1)];
}
