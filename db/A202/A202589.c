/*
 * A202589  Number of (n+2)X8 binary arrays avoiding patterns 001 and 100 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) for OEIS A202589:
   Count of (n+2)×8 binary arrays avoiding the 3‑bit patterns 001 and 100
   in rows, columns, and NW‑to‑SE diagonals.  The function memoises
   all intermediate data so any order of calls is accepted.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Structure to hold a list of successors for a pair of rows */
typedef struct {
    uint8_t *next;   /* list of next row ids that follow this pair */
    size_t len;      /* length of the list */
} trans_t;

seq_t a(idx_t n)
{
    /* number of rows in the array */
    idx_t H = n + 2;            /* H ≥ 3 for every n ≥ 1 */

    /* static data structures – built only once */
    static bool init = false;
    static size_t V = 0;                     /* number of valid rows */
    static int rowToId[256] = {0};           /* map 8‑bit pattern → id or -1 */
    static uint8_t *validRows = NULL;        /* validRows[id] = pattern */
    static size_t pairCount = 0;             /* V*V */
    static trans_t trans[65536];             /* upper bound on V² */

    /* static DP buffers – maximum size 256² = 65536 */
    static seq_t dpBuf[65536];
    static seq_t nextBuf[65536];
    seq_t *dp = dpBuf;
    seq_t *nextdp = nextBuf;

    if (!init) {
        /* 1. Build list of rows that satisfy the row constraint */
        V = 0;
        for (int r = 0; r < 256; ++r) {
            bool ok = true;
            for (int j = 0; j <= 5 && ok; ++j) {
                int b2 = (r >> (j + 2)) & 1;
                int b1 = (r >> (j + 1)) & 1;
                int b0 = (r >> j) & 1;
                int pat = (b2 << 2) | (b1 << 1) | b0;
                if (pat == 1 || pat == 4)   /* 001 or 100 */
                    ok = false;
            }
            rowToId[r] = ok ? (int)V++ : -1;
        }

        /* allocate memory for the list of valid rows */
        validRows = (uint8_t *)calloc(V, sizeof(uint8_t));
        for (int r = 0; r < 256; ++r)
            if (rowToId[r] != -1)
                validRows[rowToId[r]] = (uint8_t)r;

        pairCount = V * V;

        /* initialise transition table */
        for (size_t i = 0; i < pairCount; ++i) {
            trans[i].next = NULL;
            trans[i].len  = 0;
        }

        /* 2. Pre‑compute transitions between pairs of rows */
        for (size_t id1 = 0; id1 < V; ++id1) {
            uint8_t r1 = validRows[id1];
            for (size_t id2 = 0; id2 < V; ++id2) {
                uint8_t r2 = validRows[id2];
                size_t idx = id1 * V + id2;

                uint8_t *tmp = (uint8_t *)malloc(V * sizeof(uint8_t));
                size_t cnt = 0;
                for (size_t id3 = 0; id3 < V; ++id3) {
                    uint8_t r3 = validRows[id3];
                    bool ok = true;

                    /* vertical triple check */
                    for (int j = 0; j < 8 && ok; ++j) {
                        int a = (r1 >> j) & 1;
                        int b = (r2 >> j) & 1;
                        int c = (r3 >> j) & 1;
                        if (a == 0 && b == 0 && c == 1) ok = false;
                        if (a == 1 && b == 0 && c == 0) ok = false;
                    }

                    /* diagonal NW–SE triple check */
                    for (int j = 0; j <= 5 && ok; ++j) {
                        int a = (r1 >> (j + 2)) & 1;
                        int b = (r2 >> (j + 1)) & 1;
                        int c = (r3 >> j) & 1;
                        if (a == 0 && b == 0 && c == 1) ok = false;
                        if (a == 1 && b == 0 && c == 0) ok = false;
                    }

                    if (ok)
                        tmp[cnt++] = (uint8_t)id3;
                }

                if (cnt > 0) {
                    trans[idx].next = (uint8_t *)malloc(cnt * sizeof(uint8_t));
                    memcpy(trans[idx].next, tmp, cnt * sizeof(uint8_t));
                    trans[idx].len  = cnt;
                }
                free(tmp);
            }
        }

        init = true;
    }

    /* 3. Initialise DP: every pair of starting rows counts as 1 */
    for (size_t p = 0; p < pairCount; ++p)
        dp[p] = (seq_t)1;

    /* 4. Add rows one by one */
    for (idx_t r = 2; r < H; ++r) {
        /* clear next buffer */
        for (size_t p = 0; p < pairCount; ++p)
            nextdp[p] = (seq_t)0;

        /* transition from each current pair */
        for (size_t p = 0; p < pairCount; ++p) {
            seq_t cnt = dp[p];
            if (cnt == (seq_t)0) continue;
            size_t prevId = p % V;               /* last row of the pair */
            trans_t *t = &trans[p];
            for (size_t k = 0; k < t->len; ++k) {
                uint8_t nextId = t->next[k];
                size_t newIdx = prevId * V + nextId;
                nextdp[newIdx] += cnt;
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmpBuf = dp;
        dp = nextdp;
        nextdp = tmpBuf;
    }

    /* 5. Sum all counts – this is the answer for H rows */
    seq_t ans = (seq_t)0;
    for (size_t p = 0; p < pairCount; ++p)
        ans += dp[p];

    return ans;
}
