/*
 * A013949  Start with 1, apply 1->12, 21->21, 22->21, 2->2 (for final 2),
 *          take limit.
 * https://oeis.org/A013949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: start with the word "1".
   Repeatedly apply the following rewriting rules on adjacent symbols:
     1  → 12
     2x → 21   (for any following symbol x)
     2  (final symbol) → 2
   The limit of this process is an infinite word over {1,2}.
   a(n) is the n‑th symbol of that word (1‑based). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, but safe */

    /* start word "1" */
    unsigned char *w = (unsigned char *)malloc(1);
    if (!w)
        return 0;               /* allocation failure */
    w[0] = '1';
    size_t wlen = 1;

    /* iterate until we have at least n symbols */
    while (wlen < (size_t)n) {
        /* compute the length of the next word */
        size_t newlen = 0;
        for (size_t i = 0; i < wlen; ) {
            if (w[i] == '1') {
                newlen += 2;
                i += 1;
            } else { /* w[i] == '2' */
                if (i + 1 == wlen) {   /* final 2 */
                    newlen += 1;
                    i += 1;
                } else { /* pair 2? → 21 */
                    newlen += 2;
                    i += 2;
                }
            }
        }

        /* build the next word */
        unsigned char *next = (unsigned char *)malloc(newlen);
        if (!next) {
            free(w);
            return 0;           /* allocation failure */
        }

        size_t pa = 0;
        for (size_t i = 0; i < wlen; ) {
            if (w[i] == '1') {
                next[pa++] = '1';
                next[pa++] = '2';
                i += 1;
            } else { /* w[i] == '2' */
                if (i + 1 == wlen) {   /* final 2 */
                    next[pa++] = '2';
                    i += 1;
                } else { /* pair 2? → 21 */
                    next[pa++] = '2';
                    next[pa++] = '1';
                    i += 2;
                }
            }
        }

        free(w);
        w = next;
        wlen = pa;                 /* actual used length */
    }

    unsigned char c = w[n - 1];       /* '1' or '2' */
    seq_t res = (seq_t)(c - '0');     /* 1 ↦ 1, 2 ↦ 2 */
    free(w);
    return res;
}
