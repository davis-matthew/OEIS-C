/*
 * A164234  Number of binary strings of length n with equal numbers of 00011
 *          and 11100 substrings.
 * https://oeis.org/A164234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A164234: Number of binary strings of length n with equal numbers of
 * 00011 and 11100 substrings.
 * Computed by dynamic programming over the last four bits of the string
 * and the difference between the two substring counts.
 * For n < 5 every string is counted.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* All 2^n strings satisfy the condition for n < 5 */
    if (n < 5) {
        return (seq_t)1 << n;   /* 1, 2, 4, 8, 16 */
    }

    /* Difference can range from -n to +n, offset to handle negatives */
    const idx_t offset     = n;
    const idx_t diff_span  = 2 * n + 1;
    const idx_t state_cnt  = 16;  /* 4-bit suffix */

    /* Allocate DP tables, cast to seq_t* for C++ */
    seq_t *cur  = (seq_t *)calloc(state_cnt * diff_span, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(state_cnt * diff_span, sizeof(seq_t));
    if (!cur || !next) {
        free(cur);
        free(next);
        return 0;
    }

    /* Helper to convert (state, diff) to array index */
    #define IDX(s,d) (((s) * diff_span) + (d))

    /* Initialise after first four bits: difference is 0 for each suffix */
    for (idx_t s = 0; s < state_cnt; ++s) {
        cur[IDX(s, offset)] = (seq_t)1;
    }

    /* Pre‑compute transition data */
    int8_t  delta[16][2];
    uint8_t nxt[16][2];
    for (idx_t s = 0; s < state_cnt; ++s) {
        for (int b = 0; b <= 1; ++b) {
            uint8_t w = (uint8_t)(((s << 1) | b) & 0x1F);   /* five bits */
            if (w == 0b00011)            delta[s][b] =  1;
            else if (w == 0b11100)       delta[s][b] = -1;
            else                        delta[s][b] =  0;
            nxt[s][b] = (uint8_t)(((s << 1) | b) & 0x0F);   /* new 4‑bit state */
        }
    }

    /* DP over the remaining n-4 bits */
    for (idx_t step = 0; step < n - 4; ++step) {
        /* Clear next table */
        for (size_t i = 0; i < state_cnt * diff_span; ++i) {
            next[i] = (seq_t)0;
        }

        /* Current maximum absolute difference */
        idx_t max_diff = step;  /* difference cannot exceed number of windows seen */

        for (idx_t s = 0; s < state_cnt; ++s) {
            for (idx_t d = offset - max_diff; d <= offset + max_diff; ++d) {
                seq_t val = cur[IDX(s, d)];
                if (!val) continue;

                for (int b = 0; b <= 1; ++b) {
                    int d2 = (int)d + delta[s][b];                 /* signed */
                    idx_t nd = (idx_t)d2;                          /* guaranteed 0..diff_span-1 */
                    idx_t ns = nxt[s][b];
                    next[IDX(ns, nd)] += val;
                }
            }
        }

        /* Swap tables */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all strings where the difference is zero */
    seq_t result = (seq_t)0;
    for (idx_t s = 0; s < state_cnt; ++s) {
        result += cur[IDX(s, offset)];
    }

    free(cur);
    free(next);

    return result;
}
