/*
 * A106795  Fixed point of the morphism 1 -> 1,1,1,1,1,1,2,2,2,3; 2 ->
 *          2,2,3,1,1,1,1; 3 -> 3,1,1,1,2,2, starting with a(0) = 1.
 * https://oeis.org/A106795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of morphism
   1 → 1,1,1,1,1,1,2,2,2,3
   2 → 2,2,3,1,1,1,1
   3 → 3,1,1,1,2,2
   a(0) = 1
   The function expands the word iteratively until its length exceeds n
   and then returns the n‑th symbol. */

#include <stdlib.h>

/* a(n) – element of OEIS sequence A106795, 0‑based indexing */
seq_t a(idx_t n)
{
    /* static state shared by all calls */
    static seq_t *seq   = NULL;   /* current word */
    static idx_t seq_len = 0;     /* number of symbols in seq */

    /* constant symbols */
    static const seq_t ONE   = (seq_t)1;
    static const seq_t TWO   = (seq_t)2;
    static const seq_t THREE = (seq_t)3;

    /* initialise with the seed word 1 */
    if (seq == NULL) {
        seq_len = 1;
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = ONE;
    }

    /* expand until we have at least n+1 symbols */
    while (seq_len <= n) {
        /* compute length of the next word */
        idx_t new_len = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == ONE)      new_len += 10;
            else if (seq[i] == TWO) new_len += 7;
            else if (seq[i] == THREE) new_len += 6;
            else                     new_len += 1;   /* safeguard */
        }

        /* allocate buffer for the new word */
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;

        /* perform the substitution for each symbol */
        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == ONE) {            /* 1 → 1,1,1,1,1,1,2,2,2,3 */
                for (int j = 0; j < 6; ++j) next[pos++] = ONE;
                for (int j = 0; j < 3; ++j) next[pos++] = TWO;
                next[pos++] = THREE;
            } else if (seq[i] == TWO) {     /* 2 → 2,2,3,1,1,1,1 */
                for (int j = 0; j < 2; ++j) next[pos++] = TWO;
                next[pos++] = THREE;
                for (int j = 0; j < 4; ++j) next[pos++] = ONE;
            } else {                       /* 3 → 3,1,1,1,2,2 */
                next[pos++] = THREE;
                for (int j = 0; j < 3; ++j) next[pos++] = ONE;
                for (int j = 0; j < 2; ++j) next[pos++] = TWO;
            }
        }

        /* discard old word and store the new one */
        free(seq);
        seq = next;
        seq_len = new_len;
    }

    /* the requested term is stored in the expanded word */
    return seq[(size_t)n];
}
