/*
 * A188432  Fixed point of the morphism 0->001, 1->01.
 * https://oeis.org/A188432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 141, u32 n <= 141, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A188432: the fixed point of the morphism 0->001, 1->01.
   The function generates the fixed point word until its length is at
   least n and returns the (n‑1)st symbol.  The word is built with
   unsigned bytes; the return value is converted to seq_t.  This is
   O(n) in time and uses only trivial integer arithmetic, so it
   satisfies the performance requirement even for the arbitrary‑precision
   build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* the offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* start with the root of the fixed point: the single symbol 0 */
    unsigned char *word = (unsigned char*)malloc(1);
    if (!word) abort();   /* allocation failure, impossible for n<=10000 */
    word[0] = 0;
    idx_t   len = 1;

    while (len < n) {
        /* compute the length of the next level */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i)
            new_len += (word[i] == 0) ? 3 : 2;

        unsigned char *next = (unsigned char*)malloc(new_len);
        if (!next) abort();
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (word[i] == 0) {
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
            } else {            /* word[i] == 1 */
                next[pos++] = 0;
                next[pos++] = 1;
            }
        }

        free(word);
        word = next;
        len  = new_len;
    }

    /* the desired term is at index n-1 (0‑based) */
    seq_t result = (seq_t)word[n - 1];

    free(word);
    return result;
}
