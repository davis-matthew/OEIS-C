/*
 * A207348  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207348: number of 4×n binary arrays avoiding
    000 and 011 horizontally, and 000 and 111 vertically.
    Computed by dynamic programming over the last two columns
    (state = (prev2 << 4) | prev1).  The routine works for any
    idx_t/seq_t types provided by the build and is O(n) time. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Identify the 10 columns that satisfy the vertical constraints. */
    bool valid_col[16] = {0};
    idx_t valid_idx[16];
    size_t valid_cnt = 0;
    for (idx_t col = 0; col < 16; ++col) {
        int b0 = (int)((col >> 0) & 1);
        int b1 = (int)((col >> 1) & 1);
        int b2 = (int)((col >> 2) & 1);
        int b3 = (int)((col >> 3) & 1);
        int top    = b0 | (b1 << 1) | (b2 << 2);   /* rows 0‑2 */
        int middle = b1 | (b2 << 1) | (b3 << 2);   /* rows 1‑3 */
        if (top == 0 || top == 7) continue;        /* 000 or 111 */
        if (middle == 0 || middle == 7) continue;    /* 000 or 111 */
        valid_col[col] = true;
        valid_idx[valid_cnt++] = col;
    }

    /* a(1) = 10, a(2) = 100 (10²) */
    if (n == 1) return (seq_t)valid_cnt;       /* 10 */
    if (n == 2) {
        seq_t total = 0;
        for (size_t i = 0; i < valid_cnt; ++i) total += 1;   /* 10 × 10 = 100 */
        return (seq_t)(valid_cnt * valid_cnt);             /* 100 */
    }

    /* DP arrays */
    seq_t dp_prev[256] = {0};
    seq_t dp_next[256] = {0};

    /* Initialize after two columns (no horizontal constraints yet). */
    for (size_t i = 0; i < valid_cnt; ++i) {
        idx_t c1 = valid_idx[i];
        for (size_t j = 0; j < valid_cnt; ++j) {
            idx_t c2 = valid_idx[j];
            size_t st = ((size_t)c1 << 4) | (size_t)c2;
            dp_prev[st] += 1;
        }
    }

    /* Process columns 3 … n. */
    for (idx_t col = 3; col <= n; ++col) {
        for (size_t s = 0; s < 256; ++s) dp_next[s] = 0;
        for (size_t s = 0; s < 256; ++s) {
            seq_t ways = dp_prev[s];
            if (ways == 0) continue;
            idx_t prev2 = (s >> 4) & 0xF;
            idx_t prev1 = s & 0xF;
            for (size_t vi = 0; vi < valid_cnt; ++vi) {
                idx_t cur = valid_idx[vi];
                bool ok = true;
                /* Horizontal constraints for each of the 4 rows: forbid 000 or 011. */
                for (int r = 0; r < 4; ++r) {
                    int b0 = (int)((prev2 >> r) & 1);
                    int b1 = (int)((prev1 >> r) & 1);
                    int b2 = (int)((cur   >> r) & 1);
                    int pat = (b0 << 2) | (b1 << 1) | b2;
                    if (pat == 0 || pat == 3) { ok = false; break; }
                }
                if (!ok) continue;
                size_t next = ((size_t)prev1 << 4) | (size_t)cur;
                dp_next[next] += ways;
            }
        }
        for (size_t i = 0; i < 256; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum all final states. */
    seq_t total = 0;
    for (size_t i = 0; i < 256; ++i) total += dp_prev[i];
    return total;
}
