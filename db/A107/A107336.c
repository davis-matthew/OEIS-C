/*
 * A107336  222 Pisot cubic substitution with characteristic
 *          polynomial:x^3-2*x^2-2*x-2.
 * https://oeis.org/A107336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A107336: 222 Pisot cubic substitution
   Sequence defined by the substitution pattern
       1 → 1 2 3 3 2 1
       2 → 3
       3 → 1
   with fixed point starting from 1.  The nth term is the nth symbol
   (0‑indexed).  The implementation grows the fixed‑point word until
   it contains the requested index, then returns the symbol at that
   position.  Only additions, subtractions, and comparisons on
   seq_t are used.  No external utilities are required. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the word generated so far */
    static seq_t *word = NULL;
    static size_t sz = 0;     /* current length of word   */
    static size_t cap = 0;    /* allocated capacity       */

    /* initialise with the seed word "1"    */
    if (word == NULL) {
        cap = 64;                     /* initial capacity      */
        word = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!word) return 0;          /* out of memory         */
        word[0] = (seq_t)1;
        sz = 1;
    }

    /* grow word until it covers index n */
    while (sz <= (size_t)n) {
        /* compute new length upper‑bound: at most 6 times current */
        size_t new_cap = sz * 6;
        seq_t *new_word = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_word) return 0;      /* out of memory         */
        size_t new_sz = 0;

        /* expand each symbol according to the substitution */
        for (size_t i = 0; i < sz; ++i) {
            seq_t c = word[i];
            if (c == (seq_t)1) {
                new_word[new_sz++] = (seq_t)1;
                new_word[new_sz++] = (seq_t)2;
                new_word[new_sz++] = (seq_t)3;
                new_word[new_sz++] = (seq_t)3;
                new_word[new_sz++] = (seq_t)2;
                new_word[new_sz++] = (seq_t)1;
            } else if (c == (seq_t)2) {
                new_word[new_sz++] = (seq_t)3;
            } else { // c == 3
                new_word[new_sz++] = (seq_t)1;
            }
        }

        /* free old word and update pointers */
        free(word);
        word = new_word;
        sz   = new_sz;
        cap  = new_cap;
    }

    /* return the requested term */
    return word[(size_t)n];
}
