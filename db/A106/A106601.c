/*
 * A106601  3-limiting word of the morphism 1->2, 2->3, 3->31233.
 * https://oeis.org/A106601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   3‑limiting word of the morphism 1→2, 2→3, 3→31233
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *word = NULL;
    static idx_t word_len = 0;

    /* initialise word with the starting symbol 3 */
    if (word_len == 0) {
        word = (seq_t*)malloc(sizeof(seq_t));
        if (!word) return 0;          /* allocation failure (unlikely) */
        word[0] = (seq_t)3;
        word_len = 1;
    }

    /* expand until we have covered index n */
    while (word_len <= n) {
        idx_t new_len = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            seq_t v = word[i];
            new_len += (v == (seq_t)3 ? 5 : 1);
        }

        seq_t *new_word = (seq_t*)malloc(sizeof(seq_t) * new_len);
        if (!new_word) return 0;      /* allocation failure */

        idx_t pos = 0;
        for (idx_t i = 0; i < word_len; ++i) {
            seq_t v = word[i];
            if (v == (seq_t)3) {
                new_word[pos++] = (seq_t)3;
                new_word[pos++] = (seq_t)1;
                new_word[pos++] = (seq_t)2;
                new_word[pos++] = (seq_t)3;
                new_word[pos++] = (seq_t)3;
            } else if (v == (seq_t)1) {
                new_word[pos++] = (seq_t)2;
            } else { /* v == 2 */
                new_word[pos++] = (seq_t)3;
            }
        }

        free(word);
        word = new_word;
        word_len = new_len;
    }

    return word[n];
}
