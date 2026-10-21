/*
 * A207520  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207520: Count of 4×n binary arrays avoiding the horizontal
   patterns 001 and 010 and the vertical patterns 010 and 101.
   Computed by dynamic programming on the 10 admissible column patterns.
   Initial values: a(1) = 10, a(2) = 100. */
#include <stddef.h>
#include <stdbool.h>

/* External type definitions are provided by the build: seq_t and idx_t. */

seq_t a(idx_t n)
{
    /* Admissible 4‑bit column patterns (no forbidden vertical triples). */
    static const bool col_ok[16] = {
        /* 0000 0001 0010 0011 0100 0101 0110 0111
           1000 1001 1010 1011 1100 1101 1110 1111 */
        true,  /* 0000 */
        true,  /* 0001 */
        false, /* 0010 */
        true,  /* 0011 */
        false, /* 0100 */
        false, /* 0101 */
        true,  /* 0110 */
        true,  /* 0111 */
        true,  /* 1000 */
        true,  /* 1001 */
        false, /* 1010 */
        false, /* 1011 */
        true,  /* 1100 */
        false, /* 1101 */
        true,  /* 1110 */
        true   /* 1111 */
    };

    /* List of admissible columns as indices 0..15. */
    static int col_idx[10];
    static int col_cnt = 0;
    if (col_cnt == 0) {
        int j = 0;
        for (int i = 0; i < 16; ++i)
            if (col_ok[i])
                col_idx[j++] = i;
        col_cnt = j;          /* should become 10 */
    }

    /* Transition tables: next[a][b] gives all admissible c following pair (a,b). */
    static unsigned char trans_cnt[10][10];
    static unsigned char trans_list[10][10][10];
    static bool init = false;
    if (!init) {
        for (int a = 0; a < col_cnt; ++a) {
            for (int b = 0; b < col_cnt; ++b) {
                int cnt = 0;
                for (int c = 0; c < col_cnt; ++c) {
                    bool ok = true;
                    /* Check forbidden horizontal triples in each row. */
                    for (int r = 0; r < 4 && ok; ++r) {
                        int ba = (col_idx[a] >> r) & 1;
                        int bb = (col_idx[b] >> r) & 1;
                        int bc = (col_idx[c] >> r) & 1;
                        if ((ba == 0 && bb == 0 && bc == 1) ||
                            (ba == 0 && bb == 1 && bc == 0))
                            ok = false;
                    }
                    if (ok)
                        trans_list[a][b][cnt++] = (unsigned char)c;
                }
                trans_cnt[a][b] = (unsigned char)cnt;
            }
        }
        init = true;
    }

    /* Handle small n directly. */
    if (n == 1) return (seq_t)col_cnt;             /* 10 */
    if (n == 2) return (seq_t)col_cnt * col_cnt;   /* 100 */

    /* DP: dp_curr[a*10 + b] = number of ways ending with columns a,b. */
    seq_t dp_curr[100];
    seq_t dp_next[100];
    for (int i = 0; i < 100; ++i) dp_curr[i] = 0;

    /* All pairs allowed for length 2. */
    for (int aIdx = 0; aIdx < col_cnt; ++aIdx)
        for (int bIdx = 0; bIdx < col_cnt; ++bIdx)
            dp_curr[aIdx * 10 + bIdx] = 1;

    /* Build up to length n. */
    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < 100; ++i) dp_next[i] = 0;
        for (int aIdx = 0; aIdx < col_cnt; ++aIdx) {
            for (int bIdx = 0; bIdx < col_cnt; ++bIdx) {
                seq_t ways = dp_curr[aIdx * 10 + bIdx];
                if (ways == 0) continue;
                int tmax = trans_cnt[aIdx][bIdx];
                for (int t = 0; t < tmax; ++t) {
                    int cIdx = trans_list[aIdx][bIdx][t];
                    dp_next[bIdx * 10 + cIdx] += ways;
                }
            }
        }
        /* copy next to current */
        for (int i = 0; i < 100; ++i) dp_curr[i] = dp_next[i];
    }

    /* Sum all ending states. */
    seq_t total = 0;
    for (int i = 0; i < 100; ++i) total += dp_curr[i];
    return total;
}
