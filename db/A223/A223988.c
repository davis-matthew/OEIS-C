/*
 * A223988  Number of 3Xn 0..3 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A223988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 53, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223988: Counts of 3×n arrays over {0,1,2,3} with rows unimodal and columns nondecreasing.
   Uses dynamic programming over columns with state (last values of each row, flags indicating whether each row has started its decreasing part). */
#include <stdint.h>

typedef struct { int a, b, c; } triple_t;

static inline unsigned int encode_state(int a, int b, int c, int fmask) {
    return ((((a * 4 + b) * 4 + c) << 3) | fmask);
}

static inline void decode_state(unsigned int idx, int *a, int *b, int *c, int *fmask) {
    *fmask = idx & 7;
    unsigned int t = idx >> 3;
    *c =  t & 3;
    t >>= 2;
    *b =  t & 3;
    *a =  t >> 2;
}

static inline unsigned int next_idx_by_decision(int a, int b, int c, int fmask, const triple_t *t) {
    int nfmask = fmask;
    if ((fmask & 1) != 0) {
        if (t->a > a) return 512; /* mark invalid */
    } else {
        if (t->a < a) nfmask |= 1;
    }
    if ((fmask >> 1 & 1) != 0) {
        if (t->b > b) return 512;
    } else {
        if (t->b < b) nfmask |= 2;
    }
    if ((fmask >> 2 & 1) != 0) {
        if (t->c > c) return 512;
    } else {
        if (t->c < c) nfmask |= 4;
    }
    return encode_state(t->a, t->b, t->c, nfmask);
}

seq_t a(idx_t n) {
    /* Pre‑compute column patterns */
    static triple_t col[20];
    static int colCnt = 0;
    static uint16_t trans[512][20];
    static uint8_t  transCnt[512];
    static int    initialized = 0;
    if (!initialized) {
        /* fill column patterns */
        for (int a = 0; a <= 3; ++a)
            for (int b = a; b <= 3; ++b)
                for (int c = b; c <= 3; ++c)
                    col[colCnt++] = (triple_t){a, b, c};

        /* pre‑compute transitions for every state */
        for (int idx = 0; idx < 512; ++idx) {
            int a, b, c, fmask;
            decode_state(idx, &a, &b, &c, &fmask);
            transCnt[idx] = 0;
            for (int tIdx = 0; tIdx < colCnt; ++tIdx) {
                int nIdx = next_idx_by_decision(a, b, c, fmask, &col[tIdx]);
                if (nIdx != 512) {
                    trans[idx][transCnt[idx]++] = (uint16_t)nIdx;
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays */
    seq_t cur[512], nxt[512];
    int i;
    for (i = 0; i < 512; ++i) cur[i] = 0;
    /* first column: all flags are 0 */
    for (int tIdx = 0; tIdx < colCnt; ++tIdx) {
        unsigned int idx = encode_state(col[tIdx].a, col[tIdx].b, col[tIdx].c, 0);
        cur[idx] = cur[idx] + 1;
    }

    /* iterate remaining columns */
    for (idx_t col = 2; col <= n; ++col) {
        for (i = 0; i < 512; ++i) nxt[i] = 0;
        for (i = 0; i < 512; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            uint8_t tcnt = transCnt[i];
            for (uint8_t j = 0; j < tcnt; ++j) {
                unsigned int next = trans[i][j];
                nxt[next] = nxt[next] + val;
            }
        }
        for (i = 0; i < 512; ++i) cur[i] = nxt[i];
    }

    /* sum all terminal states */
    seq_t ans = 0;
    for (i = 0; i < 512; ++i) ans = ans + cur[i];
    return ans;
}
