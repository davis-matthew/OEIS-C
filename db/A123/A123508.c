/*
 * A123508  1-dimensional quasiperiodic heptagonal sequence.
 * https://oeis.org/A123508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth term of the 1‑dimensional quasiperiodic
 *            heptagonal sequence produced by iterated substitution: 1→3 ;
 *            2→2,3 ; 3→1,3,2. The sequence is the concatenation of all
 *            successive iterates starting from the single symbol 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial iterate: [1] */
    idx_t cur_len = 1;
    seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
    if (!cur) exit(1);          /* allocation failure */
    cur[0] = 1;

    idx_t cumulative = 0;       /* number of symbols already produced */

    for (;;) {
        /* does the nth symbol lie in the current iterate? */
        if (cumulative + cur_len > n) {
            idx_t offset = n - cumulative;
            seq_t val = cur[offset];
            free(cur);
            return val;
        }

        cumulative += cur_len;

        /* compute length of next iterate */
        idx_t next_len = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            seq_t v = cur[i];
            if (v == 1)       next_len += 1;
            else if (v == 2)  next_len += 2;
            else              next_len += 3;   /* v == 3 */
        }

        /* allocate memory for next iterate */
        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!next) exit(1);  /* allocation failure */

        /* fill next iterate with the substitutions */
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            seq_t v = cur[i];
            if (v == 1) {
                next[pos++] = 3;
            } else if (v == 2) {
                next[pos++] = 2;
                next[pos++] = 3;
            } else { /* v == 3 */
                next[pos++] = 1;
                next[pos++] = 3;
                next[pos++] = 2;
            }
        }

        /* move to next iterate */
        free(cur);
        cur = next;
        cur_len = next_len;
    }
}
