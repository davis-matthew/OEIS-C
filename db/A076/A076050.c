/*
 * A076050  Limiting sequence if we start with 2 and successively replace n
 *          with 2,3,4,...,n,n+1.
 * https://oeis.org/A076050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A007001(n) + 1 = the nth symbol in the morphic word
 *            obtained by starting with 2 and replacing each symbol x by the
 *            block 2,3,…,x,x+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached word (1‑based positions) */
static seq_t *word = NULL;
static idx_t  size = 0;   /* current number of symbols            */

/* Return the nth term of A076050 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range – undefined behaviour  */
        return 0;

    /* --- initialise with the single symbol 2 --------------- */
    if (!word) {
        word = (seq_t *)malloc(sizeof(seq_t));  /* size == 1 */
        if (!word) return 0;                    /* allocation failed */
        word[0] = (seq_t)2;
        size = 1;
    }

    /* --- grow the word until it contains at least n symbols  */
    while (size < n) {
        /* compute the length of the next word (sum of current symbols) */
        idx_t newlen = 0;
        for (idx_t i = 0; i < size; ++i)
            newlen += (idx_t)word[i];

        /* allocate new buffer for the expanded word */
        seq_t *new_word = (seq_t *)malloc(newlen * sizeof(seq_t));
        if (!new_word) return 0;   /* allocation failed */

        /* fill new_word with the replacement blocks 2,…,val+1 */
        idx_t pos = 0;
        for (idx_t i = 0; i < size; ++i) {
            idx_t val = (idx_t)word[i];
            for (idx_t v = 2; v <= val + 1; ++v)
                new_word[pos++] = (seq_t)v;
        }

        /* replace old word */
        free(word);
        word = new_word;
        size = newlen;
    }

    /* --- return the required symbol (1‑based) -------------- */
    return word[n - 1];
}
