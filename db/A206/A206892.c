/*
 * A206892  Number of 6Xn 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary matrices avoiding horizontal 010 *
 *            and vertical 100
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   valid_vertical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/valid_vertical.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* Enumerate all 6‑bit masks satisfying the vertical constraint */
    uint8_t valid_masks[64];
    idx_t V = 0;
    for (uint8_t m = 0; m < 64; ++m) {
        if (valid_vertical(m))
            valid_masks[V++] = m;
    }

    if (n == 1)
        return (seq_t)V;          /* one column, any valid mask */
    if (n == 2)
        return (seq_t)V * (seq_t)V;/* two columns, any pair of valid masks */

    /* State: pair of consecutive columns (prev, cur) */
    idx_t S = V * V;              /* number of states */

    /* DP arrays holding counts for each state */
    seq_t *dp_cur = (seq_t*)malloc(S * sizeof(seq_t));
    seq_t *dp_next = (seq_t*)malloc(S * sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur);
        free(dp_next);
        return 0;
    }
    memset(dp_cur, 0, S * sizeof(seq_t));
    memset(dp_next, 0, S * sizeof(seq_t));

    /* Preallocate transition lists for each state */
    struct TransNode { idx_t *to; unsigned cap; unsigned used; };
    struct TransNode *trans = (struct TransNode*)malloc(S * sizeof(struct TransNode));
    if (!trans) {
        free(dp_cur);
        free(dp_next);
        return 0;
    }
    for (idx_t i = 0; i < S; ++i) {
        trans[i].cap = (unsigned)V;          /* initial guess */
        trans[i].used = 0;
        trans[i].to = (idx_t*)malloc(trans[i].cap * sizeof(idx_t));
        if (!trans[i].to) {
            for (idx_t k = 0; k < i; ++k) free(trans[k].to);
            free(trans);
            free(dp_cur);
            free(dp_next);
            return 0;
        }
    }

    /* Build transitions: from state (p,c) to state (c,d) */
    for (idx_t i = 0; i < V; ++i) {
        uint8_t p = valid_masks[i];
        for (idx_t j = 0; j < V; ++j) {
            uint8_t c = valid_masks[j];
            idx_t state = i * V + j;
            for (idx_t dIdx = 0; dIdx < V; ++dIdx) {
                uint8_t d = valid_masks[dIdx];
                /* Horizontal check: no 0 1 0 across p, c, d */
                uint8_t notp = (~p) & 0x3F;
                uint8_t notd = (~d) & 0x3F;
                if ((notp & c & notd) != 0)
                    continue;                 /* forbidden triple */
                idx_t next = j * V + dIdx;
                /* Grow array if needed */
                if (trans[state].used == trans[state].cap) {
                    trans[state].cap *= 2;
                    idx_t *new_to = (idx_t*)realloc(trans[state].to,
                                                    trans[state].cap * sizeof(idx_t));
                    if (!new_to) {
                        for (idx_t k = 0; k < i; ++k) free(trans[k].to);
                        free(trans);
                        free(dp_cur);
                        free(dp_next);
                        return 0;
                    }
                    trans[state].to = new_to;
                }
                trans[state].to[trans[state].used++] = next;
            }
        }
    }

    /* Initial DP: all pairs of columns are allowed */
    for (idx_t i = 0; i < S; ++i) dp_cur[i] = 1;

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        memset(dp_next, 0, S * sizeof(seq_t));
        for (idx_t state = 0; state < S; ++state) {
            seq_t val = dp_cur[state];
            if (!val) continue;
            struct TransNode *t = &trans[state];
            for (unsigned k = 0; k < t->used; ++k) {
                idx_t ns = t->to[k];
                dp_next[ns] += val;
            }
        }
        /* swap pointers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all ending states to obtain the total count */
    seq_t total = 0;
    for (idx_t i = 0; i < S; ++i)
        total += dp_cur[i];

    /* Clean up */
    for (idx_t i = 0; i < S; ++i) free(trans[i].to);
    free(trans);
    free(dp_cur);
    free(dp_next);

    return total;
}
