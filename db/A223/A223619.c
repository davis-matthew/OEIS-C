/*
 * A223619  Number of nX7 0..1 arrays with rows and columns unimodal.
 * https://oeis.org/A223619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary matrices with rows and columns
 *            unimodal
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 58, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define STATE_COUNT 2187   /* number of valid (startMask, endMask) pairs */
#define PATTERN_COUNT 29   /* number of unimodal binary strings of length 7 */
#define ID_BITS 7          /* bits used for column masks */
#define ID_SIZE (1 << (ID_BITS * 2))

/* static tables, built once */
static int patternMask[PATTERN_COUNT];
static int startMask[STATE_COUNT];
static int endMask[STATE_COUNT];
static int stateIndex[ID_SIZE];
static int16_t trans[STATE_COUNT][PATTERN_COUNT];
static int init_ready = 0;

/* build tables the first time the function is called */
static void init_tables(void)
{
    /* generate all 29 unimodal masks (0 bitmask + 28 intervals) */
    int p = 0;
    patternMask[p++] = 0;                       /* all zeros */
    for (int l = 0; l < 7; ++l)
        for (int r = l; r < 7; ++r) {
            int m = 0;
            for (int k = l; k <= r; ++k) m |= 1 << k;
            patternMask[p++] = m;
        }

    /* build mapping from (startMask, endMask) -> state index */
    for (int i = 0; i < ID_SIZE; ++i) stateIndex[i] = -1;
    int sidx = 0;
    for (int sm = 0; sm < 128; ++sm)
        for (int em = 0; em < 128; ++em)
            if ((em & ~sm) == 0) {
                startMask[sidx] = sm;
                endMask[sidx]   = em;
                stateIndex[(sm << ID_BITS) | em] = sidx;
                ++sidx;
            }
    /* sanity check: should be 2187 */
    (void)sidx;  /* suppress unused variable warning */

    /* build transition table */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int sm = startMask[s];
        int em = endMask[s];
        for (int pi = 0; pi < PATTERN_COUNT; ++pi) {
            int col = patternMask[pi];
            if (col & em) {                 /* 1 after block ended -> impossible */
                trans[s][pi] = -1;
                continue;
            }
            int nsm = sm | col;
            int nend = em | ((~col & 0x7F) & sm);
            int nid = (nsm << ID_BITS) | nend;
            trans[s][pi] = stateIndex[nid];
        }
    }
    init_ready = 1;
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not part of the offset, but harmless */

    if (!init_ready) init_tables();

    /* DP arrays */
    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];
    seq_t *cur = dp;
    seq_t *nxt = ndp;

    /* initialise */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 0;
    cur[0] = 1;   /* empty state */

    for (idx_t row = 0; row < n; ++row) {
        /* clear next row buffer */
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        /* transition from current to next */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            const int16_t *trow = trans[s];
            for (int pi = 0; pi < PATTERN_COUNT; ++pi) {
                int next = trow[pi];
                if (next < 0) continue;
                nxt[next] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all reachable states */
    seq_t res = 0;
    for (int i = 0; i < STATE_COUNT; ++i) res += cur[i];
    return res;
}
