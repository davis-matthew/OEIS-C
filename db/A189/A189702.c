/*
 * A189702  Fixed point of the morphism 0->011, 1->10.
 * https://oeis.org/A189702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism 0->011 , 1->10, starting with 0.
   Generates the infinite word 0,1,1,0,1,0,1,0,0,1,1,1,...
   The function a(n) returns the n‑th symbol (1‑based).  For n >= 1.   */

#include <stdlib.h>

static seq_t  *word = NULL;      /* Current word buffer                      */
static size_t  word_len = 0;     /* Number of symbols stored in word         */

/* Expand the current word by applying the morphism until its length
   is at least n.  This function is called from a() and updates the
   static buffer. */
static void ensure_word_len(idx_t n)
{
    while ((idx_t)word_len < n) {
        /* Count zeros and ones to compute new length          */
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0)
                ++zeros;
            else
                ++ones;
        }
        size_t new_len = zeros * 3 + ones * 2;

        seq_t *new_word = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_word) {
            /* Out of memory – aborting will cause program to terminate. */
            abort();
        }
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
            } else { /* word[i] == 1 */
                new_word[pos++] = 1;
                new_word[pos++] = 0;
            }
        }
        /* Replace old buffer.  The old buffer is freed to keep memory
           footprint small. */
        free(word);
        word     = new_word;
        word_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Lazy initialization of the starting word "0". */
    if (!word) {
        word     = (seq_t *)malloc(sizeof(seq_t));
        word[0]  = 0;
        word_len = 1;
    }

    /* Ensure the word is long enough to index n. */
    ensure_word_len(n);

    /* n is 1‑based; convert to 0‑based. */
    return word[(size_t)(n - 1)];
}
