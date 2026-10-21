/*
 * A212810  Iterate the morphism 1->122, 2->1112 starting with 1.
 * https://oeis.org/A212810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1531, u16 n <= 1531, u32 n <= 1531, u64 n <= 1531,
 *            u128 n <= 1531, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212810: iterate morphism 1->122, 2->1112 starting with 1 */
#include <stdlib.h>

static seq_t *word = NULL;
static size_t word_len = 0;

static void extend_word(void) {
    size_t next_len = 0;
    for (idx_t i = 0; i < (idx_t)word_len; ++i) {
        if (word[i] == 1)
            next_len += 3;   /* 1 -> 122 */
        else
            next_len += 4;   /* 2 -> 1112 */
    }

    seq_t *next = (seq_t*)malloc(next_len * sizeof(seq_t));
    size_t pos = 0;
    for (idx_t i = 0; i < (idx_t)word_len; ++i) {
        if (word[i] == 1) {
            next[pos++] = (seq_t)1;
            next[pos++] = (seq_t)2;
            next[pos++] = (seq_t)2;
        } else { /* word[i] == 2 */
            next[pos++] = (seq_t)1;
            next[pos++] = (seq_t)1;
            next[pos++] = (seq_t)1;
            next[pos++] = (seq_t)2;
        }
    }

    free(word);
    word = next;
    word_len = next_len;
}

seq_t a(idx_t n) {
    if (n == 0) /* offset is 1; n >= 1 for valid queries */
        return 0;

    if (word == NULL) {
        word = (seq_t*)malloc(sizeof(seq_t));
        word[0] = (seq_t)1;
        word_len = 1;
    }

    while ((idx_t)word_len < n)
        extend_word();

    return word[n - 1];
}
