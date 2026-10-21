/*
 * A071049  Number of 1's in n-th generation of 1-D CA using Rule 110, started
 *          with a single 1.
 * https://oeis.org/A071049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 426, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A071049: Number of 1's in n-th generation of 1‑D CA Rule 110,
   starting from a single 1 at generation 0.
   Computes the value by direct simulation of the cellular automaton.
   Uses a simple dynamic array of uint8_t to store the current row.
*/
#include <stdlib.h>
#include <stdint.h>

static const uint8_t rule[8] = {0,1,1,1,0,1,1,0};  /* Rule 110 table */

seq_t a(idx_t n) {
    /* static state */
    static seq_t *memo = NULL;          /* memoized a(k) values */
    static idx_t memo_len = 0;          /* number of stored values */
    static idx_t memo_cap = 0;          /* allocated capacity */
    static uint8_t *row = NULL;         /* current row bits */
    static size_t row_len = 0;          /* length of current row */
    static idx_t cur_n = 0;             /* index of current row */

    /* initialization for the first call */
    if (memo == NULL) {
        /* a(0) = 1 */
        memo_cap = 64;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        memo[0] = (seq_t)1;
        memo_len = 1;
        /* single 1 in the first row */
        row_len = 1;
        row = (uint8_t*)malloc(row_len * sizeof(uint8_t));
        row[0] = 1;
        cur_n = 0;
    }

    /* If the requested n is already computed, return it */
    if (n < memo_len) {
        return memo[n];
    }

    /* Compute rows up to n */
    for (idx_t step = cur_n; step < n; ++step) {
        size_t cur_len = row_len;
        size_t next_len = cur_len + 2;
        uint8_t *next = (uint8_t*)malloc(next_len * sizeof(uint8_t));
        size_t cnt = 0;

        for (size_t j = 0; j < next_len; ++j) {
            uint8_t L, C, R;

            if (j >= 2 && j-2 < cur_len)      L = row[j-2]; else L = 0;
            if (j >= 1 && j-1 < cur_len)      C = row[j-1]; else C = 0;
            if (j < cur_len)                  R = row[j];   else R = 0;

            uint8_t pattern = (L<<2) | (C<<1) | R;
            next[j] = rule[pattern];
            if (next[j]) ++cnt;
        }
        free(row);
        row = next;
        row_len = next_len;

        /* Expand memo array if needed */
        if (memo_len >= memo_cap) {
            size_t new_cap = memo_cap ? memo_cap * 2 : 64;
            seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
            memo = tmp;
            memo_cap = new_cap;
        }
        /* Store the count for the new row (generation step+1) */
        memo[++memo_len - 1] = (seq_t)cnt;
    }
    cur_n = n;
    return memo[n];
}
