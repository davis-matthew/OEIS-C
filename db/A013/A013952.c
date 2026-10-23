/*
 * A013952  Start with 1, apply 1->12, 21->21, 22->21, 2->2 (for final 2);
 *          take limit; note positions of 2's.
 * https://oeis.org/A013952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013952: start with the word “1”, then repeat the rules
 *   1  → 12
 *   22 → 21
 *   21 → 21
 *   2  (isolated) → 2
 *   a(n) is the position of the n‑th ‘2’ in the limit word. */
 
#include <stdlib.h>
#include <stdint.h>
 
seq_t a(idx_t n)
{
    static uint8_t *cur = NULL;
    static size_t   cur_len = 0;
    static seq_t   *pos = NULL;
    static size_t   pos_len = 0, pos_cap = 0;
    if (cur == NULL) {
        cur = (uint8_t *)malloc(1);
        cur[0] = 1;           /* word starts with “1” */
        cur_len = 1;
    }
    /* grow until we have at least n 2‑positions */
    while (pos_len < (size_t)n) {
        size_t old_len = cur_len;
        size_t nxt_cap = old_len * 2 + 16; /* safe upper bound */
        uint8_t *next = (uint8_t *)malloc(nxt_cap);
        size_t j = 0;        /* index in next */

        for (size_t i = 0; i < old_len; ++i) {
            if (cur[i] == 1) {
                /* 1 → 12 */
                next[j++] = 1;
                next[j++] = 2;
            } else { /* cur[i] == 2 */
                if (i + 1 < old_len) {
                    /* pair 21 or 22 → 21 */
                    next[j++] = 2;
                    next[j++] = 1;
                    ++i;            /* skip second symbol of the pair */
                } else {
                    /* isolated 2 → 2 */
                    next[j++] = 2;
                }
            }
        }
        size_t new_len = j;

        /* record new 2‑positions (indices > old_len) */
        for (size_t k = old_len; k < new_len; ++k) {
            if (next[k] == 2) {
                if (pos_len >= pos_cap) {
                    pos_cap = pos_cap ? pos_cap * 2 : 8;
                    pos = (seq_t *)realloc(pos, pos_cap * sizeof(seq_t));
                }
                pos[pos_len++] = (seq_t)(k + 1);   /* 1‑based position */
            }
        }

        free(cur);
        cur = next;
        cur_len = new_len;
    }
    return pos[n - 1];
}
