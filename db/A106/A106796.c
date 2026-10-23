/*
 * A106796  Fixed point of the morphism 1 -> 1,1,2; 2 -> 3; 3 -> 1,4; 4 -> 1,
 *          starting with a(0) = 1.
 * https://oeis.org/A106796
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

/* This function implements the morphism
   1 → 1,1,2
   2 → 3
   3 → 1,4
   4 → 1
   starting with a(0) = 1.  It expands the word until the required
   index n is reached and then returns the symbol at that position. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for the expanded word.  It grows only as far as
       needed for the maximum n requested in any call. */
    static seq_t *word = NULL;     /* pointer to the current expansion */
    static idx_t len   = 0;        /* current length of *word */

    /* Initialise with the starting symbol 1 on the very first call. */
    if (word == NULL) {
        len = 1;
        word = (seq_t *)malloc(sizeof(seq_t));
        if (!word) return (seq_t)0;      /* allocation failure */
        word[0] = (seq_t)1;              /* a(0) = 1 */
    }

    /* Expand until the word is long enough to contain position n. */
    while (len <= n) {
        /* Determine how much space the next expansion will need. */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i) {
            seq_t s = word[i];
            if (s == (seq_t)1)        new_len += 3;   /* 1 → 1,1,2 */
            else if (s == (seq_t)2)   new_len += 1;   /* 2 → 3   */
            else if (s == (seq_t)3)   new_len += 2;   /* 3 → 1,4 */
            else /* s == 4 */          new_len += 1;   /* 4 → 1   */
        }

        /* Allocate space for the new expansion. */
        seq_t *new_word = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_word) return (seq_t)0;  /* allocation failure */

        /* Build the expanded word. */
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            seq_t s = word[i];
            if (s == (seq_t)1) {
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)2;
            } else if (s == (seq_t)2) {
                new_word[pos++] = (seq_t)3;
            } else if (s == (seq_t)3) {
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)4;
            } else /* s == 4 */ {
                new_word[pos++] = (seq_t)1;
            }
        }

        /* Replace the old word with the expanded one. */
        free(word);
        word = new_word;
        len  = new_len;
    }

    return word[n];
}
