/*
 * A214640  A ternary sequence : closed under 1 -> 123, 2 -> 12, 3 -> 2 .
 *          Start with 1 .
 * https://oeis.org/A214640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(1)=1; the infinite word is produced by repeatedly
 * applying the substitutions 1→123, 2→12, 3→2 to the current word,
 * beginning with the single symbol 1.  The nth symbol (1‑based) of
 * this word is a(n).
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static uint8_t *word = NULL;   /* current word */
    static size_t len   = 0;       /* number of valid symbols */

    /* initialise with the seed word “1” */
    if (word == NULL) {
        word = (uint8_t *)malloc(32 * sizeof(uint8_t));
        if (word == NULL) return (seq_t)0;   /* allocation failed */
        word[0] = 1;
        len = 1;
    }

    /* grow the word until it contains at least n symbols */
    while (len < (size_t)n) {
        size_t old_len = len;
        /* each symbol expands to at most 3 symbols */
        size_t new_cap = old_len * 3 + 10;
        uint8_t *next = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
        if (next == NULL) return (seq_t)0;   /* allocation failed */

        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            uint8_t v = word[i];
            if (v == 1) {
                next[pos++] = 1;
                next[pos++] = 2;
                next[pos++] = 3;
            } else if (v == 2) {
                next[pos++] = 1;
                next[pos++] = 2;
            } else {            /* v == 3 */
                next[pos++] = 2;
            }
        }
        free(word);              /* discard old word */
        word = next;
        len  = pos;
    }

    return (seq_t)word[(size_t)n - 1];
}
