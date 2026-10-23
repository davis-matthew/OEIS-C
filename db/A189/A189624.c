/*
 * A189624  Fixed point of the morphism 0->001, 1->10.
 * https://oeis.org/A189624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fixed point of morphism 0→001, 1→10.
   a(n) is the nth symbol (1‑indexed) of the infinite word. */

#include <stdlib.h>

static seq_t *word = NULL;   /* word currently stored (prefix of the limit)      */
static idx_t  word_len = 0;  /* number of symbols in |word|                    */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n==0 is out of bounds – return 0 */
        return 0;

    /* initialise with the seed word "0" if not already done */
    if (word_len == 0) {
        word_len = 1;
        word = (seq_t *)malloc(sizeof(seq_t) * (size_t)word_len);
        if (!word) return 0;          /* allocation failed – return 0 as a fallback */
        word[0] = 0;
    }

    /* generate successive morphism images until we have at least n symbols   */
    while (word_len < n) {
        /* compute the length of the next generation */
        idx_t next_len = 0;
        for (idx_t i = 0; i < word_len; ++i)
            next_len += (word[i] == 0) ? 3 : 2;

        seq_t *next_word = (seq_t *)malloc(sizeof(seq_t) * (size_t)next_len);
        if (!next_word) return 0;          /* allocation failed */

        /* build the next generation */
        idx_t pos = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            if (word[i] == 0) {            /* 0 → 001 */
                next_word[pos++] = 0;
                next_word[pos++] = 0;
                next_word[pos++] = 1;
            } else {                       /* 1 → 10  */
                next_word[pos++] = 1;
                next_word[pos++] = 0;
            }
        }

        /* replace the old word with the new one */
        free(word);
        word = next_word;
        word_len = next_len;
    }

    /* word_len ≥ n – return the nth symbol (0‑indexed in array) */
    return word[n - 1];
}
