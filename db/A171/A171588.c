/*
 * A171588  The Pell word: Fixed point of the morphism 0->001, 1->0.
 * https://oeis.org/A171588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(1 - 1/sqrt(2))) - floor(n*(1 - 1/sqrt(2)))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iteratively applying the morphism 0 → 001, 1 → 0 and
   caching the resulting word. */

/* The function must be linked with external linkage. */
#include <stdlib.h>

/* Static cache of the morphic word. */
static char *word = NULL;         /* word buffer */
static size_t word_len = 0;       /* number of characters in buffer */

/* Return the n‑th term of A171588 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {                /* offset starts at 1; behaviour below is irrelevant */
        return (seq_t)0;
    }

    /* Initialise the cache with the starting word "0". */
    if (word == NULL) {
        word = (char*)malloc(1);          /* cast for C++ compatibility */
        if (word == NULL) return (seq_t)0;
        word[0] = '0';
        word_len = 1;
    }

    /* Grow the cached word until it covers index n. */
    while (word_len < (size_t)n) {
        /* First compute the length of the next word. */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i) {
            new_len += (word[i] == '0') ? 3 : 1;
        }

        /* Allocate new buffer. */
        char *new_word = (char*)malloc(new_len);
        if (new_word == NULL) return (seq_t)0;

        /* Build the next word by applying the morphism. */
        size_t j = 0;
        for (size_t i = 0; i < word_len; ++i) {
            if (word[i] == '0') {
                new_word[j++] = '0';
                new_word[j++] = '0';
                new_word[j++] = '1';
            } else { /* word[i] == '1' */
                new_word[j++] = '0';
            }
        }

        /* Replace old buffer. */
        free(word);
        word = new_word;
        word_len = new_len;
    }

    /* Return the requested term as seq_t (0 or 1). */
    return (seq_t)(word[n - 1] - '0');
}
