/*
 * A223866  Number of 3Xn 0..3 arrays with rows and antidiagonals unimodal and
 *          columns nondecreasing.
 * https://oeis.org/A223866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 54, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223866: number of 3×n 0..3 arrays whose rows and antidiagonals are unimodal
   and whose columns are nondecreasing.  Computed by dynamic programming. */

#include <stdbool.h>
#include <string.h>

/* constants */
enum { DUMMY_IDX = 20 };                    /* index for the dummy column */
enum { COL_COUNT = 21 };                    /* 20 real columns + dummy */
enum { STATE_COUNT = 3360 };                /* 21 * 20 * 8  (a,b,mask) */
enum { MAX_TRANS_PER_STATE = 20 };          /* at most 20 target columns */

/* triple of 3 nondecreasing values 0..3 */
struct Triple { int v[3]; };

/* precomputed all triples where 0<=a<=b<=c<=3 */
static struct Triple triples[20];

/* transition tables */
static int trans_start[STATE_COUNT];
static int trans_len[STATE_COUNT];
static int trans_data[STATE_COUNT * MAX_TRANS_PER_STATE];

/* mapping from state components to index */
static inline int state_index(int a, int b, int mask)
{
    return (((a * 20) + b) * 8) + mask;
}

/* prepare all triples once */
static void init_triples(void)
{
    int idx = 0;
    for (int a = 0; a <= 3; ++a)
        for (int b = a; b <= 3; ++b)
            for (int c = b; c <= 3; ++c)
                triples[idx++] = (struct Triple){a, b, c};
}

/* fill transition tables once */
static void precompute_transitions(void)
{
    int offset = 0;
    for (int a = 0; a < COL_COUNT; ++a) {          /* a in [0..20] (20 real + dummy) */
        for (int b = 0; b < 20; ++b) {             /* b in [0..19] (real columns only) */
            for (int mask = 0; mask < 8; ++mask) {
                int sidx = state_index(a, b, mask);
                int count = 0;
                int *out = trans_data + offset;

                const struct Triple *A = (a == DUMMY_IDX) ? NULL : &triples[a];
                const struct Triple *B = &triples[b];

                for (int c = 0; c < 20; ++c) {        /* try every possible new column */
                    const struct Triple *C = &triples[c];

                    /* unimodality of the antidiagonal of length 3 */
                    bool valley_ok = true;
                    if (a != DUMMY_IDX) {
                        if (C->v[0] > B->v[1] && A->v[2] > B->v[1])
                            valley_ok = false;
                    }

                    if (!valley_ok) continue;

                    /* unimodality of each row */
                    int newMask = 0;
                    bool ok = true;
                    for (int r = 0; r < 3; ++r) {
                        bool inc = (mask >> r) & 1;
                        int prev = B->v[r];
                        int cur  = C->v[r];
                        if (inc) {
                            if (cur >= prev)
                                newMask |= 1 << r;   /* still ascending */
                            /* else: switch to descending, bit stays 0 */
                        } else {
                            if (cur > prev) {       /* would increase after descending */
                                ok = false;
                                break;
                            }
                        }
                    }
                    if (!ok) continue;

                    int nidx = state_index(b, c, newMask);
                    out[count++] = nidx;
                }

                trans_start[sidx] = offset;
                trans_len[sidx]   = count;
                offset += count;
            }
        }
    }
}

/* ensure one-time initialization */
static bool initialized = false;
static void ensure_init(void)
{
    if (!initialized) {
        init_triples();
        precompute_transitions();
        initialized = true;
    }
}

/* sequence entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    ensure_init();

    /* DP arrays */
    static seq_t dp[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];
    seq_t *cur = dp;
    seq_t *next = nxt;

    /* clear DP arrays */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 0;
    for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

    /* initial state: only the first column present, all rows ascending */
    for (int c = 0; c < 20; ++c) {
        int idx = state_index(DUMMY_IDX, c, 7);
        cur[idx] = 1;
    }

    /* iterate columns 2..n */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        /* process transitions */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;
            int len = trans_len[s];
            int *ptr = trans_data + trans_start[s];
            for (int t = 0; t < len; ++t) {
                int ns = ptr[t];
                next[ns] += cnt;
            }
        }

        /* swap current and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < STATE_COUNT; ++i) result += cur[i];
    return result;
}
