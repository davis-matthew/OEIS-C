/*
 * A189661  Fixed point of the morphism 0->010, 1->10 starting with 0.
 * https://oeis.org/A189661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Validated: partial, pass, 144 terms through n = 144 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A189661: the nth term of the fixed point of the morphism 0→010, 1→10,
   starting from the word "0".  */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* 1‑based index: a(1) = 0 */
    if (n <= (idx_t)1) {
        return 0;
    }

    /* work with normal C pointer types for the buffer */
    size_t target = (size_t)n;
    size_t len = 1;
    seq_t *cur = mem_new(seq_t, len);
    if (!cur) return 0;          /* fallback on allocation failure */
    cur[0] = 0;                  /* starting word */

    /* iterate the morphism until we have at least the target position */
    while (len < target) {
        /* compute new length */
        size_t len2 = 0;
        for (size_t i = 0; i < len; ++i) {
            len2 += (cur[i] == 0) ? 3 : 2;
        }

        seq_t *next = mem_new(seq_t, len2);
        if (!next) { mem_free(cur); return 0; }

        /* apply the substitution */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i] == 0) {
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            } else {
                next[pos++] = 1;
                next[pos++] = 0;
            }
        }

        mem_free(cur);
        cur = next;
        len = len2;
    }

    /* return the requested symbol */
    seq_t result = cur[target - 1];
    mem_free(cur);
    return result;
}
