/*
 * A057985  Start with 0 and repeatedly substitute: 0->01, 1->12, 2->0.
 * https://oeis.org/A057985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A057985 (fixed point of 0→01, 1→12, 2→0) by iteratively
   expanding the current word until its length reaches n.  The sequence
   is stored statically so that subsequent calls do not recompute
   previously generated terms.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: current word and its length */
    static seq_t *word = NULL;
    static idx_t  len   = 0;   /* number of symbols in word */

    /* 0‑based target position (OEIS is 1‑based) */
    idx_t pos = n - 1;

    /* Initialise with the starting symbol 0 on first call */
    if (word == NULL) {
        word = (seq_t*)malloc(sizeof(seq_t));
        word[0] = (seq_t)0;      /* first term */
        len = 1;
    }

    /* Expand until we have at least n symbols */
    while (len <= pos) {
        /* Count occurrences of each symbol to compute new length */
        idx_t c0 = 0, c1 = 0, c2 = 0;

        for (idx_t i = 0; i < len; ++i) {
            seq_t v = word[i];
            if (v == (seq_t)0)       ++c0;
            else if (v == (seq_t)1)  ++c1;
            else                     ++c2;   /* v == 2 */
        }

        /* New length: 0->2, 1->2, 2->1 */
        idx_t newlen = c0 * 2 + c1 * 2 + c2 * 1;

        /* Allocate new word */
        seq_t *newword = (seq_t*)malloc(sizeof(seq_t) * newlen);

        /* Build new word by applying the morphism */
        idx_t j = 0;
        for (idx_t i = 0; i < len; ++i) {
            seq_t v = word[i];
            if (v == (seq_t)0) {           /* 0 → 0 1 */
                newword[j++] = (seq_t)0;
                newword[j++] = (seq_t)1;
            } else if (v == (seq_t)1) {    /* 1 → 1 2 */
                newword[j++] = (seq_t)1;
                newword[j++] = (seq_t)2;
            } else {                       /* 2 → 0 */
                newword[j++] = (seq_t)0;
            }
        }

        /* Replace old word */
        free(word);
        word = newword;
        len  = newlen;
    }

    return word[pos];   /* return the n‑th element (1‑based) */
}
