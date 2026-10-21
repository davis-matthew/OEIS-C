/*
 * A207775  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by DP over column patterns (columns of width 5,
 *            avoiding 0 0 1 and 0 1 0 vertically and avoiding horizontal
 *            triples 0 0 0 / 1 1 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   zero_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/zero_seq_array.h"

/* Types supplied by the build */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Constants */
#define STATE_BITS   10                     /* 5 bits for each of 2 previous cols */
#define STATE_COUNT (1 << STATE_BITS)       /* 1024 */
#define PATTERN_BITS 5
#define PATTERN_COUNT (1 << PATTERN_BITS)   /* 32 */
#define MAX_TRANS    14                    /* maximum outgoing transitions per state */

/* Static tables (initialized once) */
static int  allowed[PATTERN_COUNT];          /* allowed column bitmasks */
static int  allowed_cnt = 0;

/* transitions: for each state s, a list of next states */
static uint16_t trans_next[STATE_COUNT][MAX_TRANS];
static int      trans_cnt[STATE_COUNT];

/* DP arrays for current and next lengths */
static seq_t dp[STATE_COUNT];
static seq_t newdp[STATE_COUNT];

/* Flag to ensure one‑time initialization */
static int initialised = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Guard against n == 0 (offset starts at 1) */
    if (n == 0) return 0;

    /* ------- one‑time table construction ------- */
    if (!initialised) {
        /* 1. enumerate allowed column patterns (vertical restriction) */
        for (int pat = 0; pat < PATTERN_COUNT; ++pat) {
            int ok = 1;
            for (int r = 0; r <= 2; ++r) {          /* three consecutive rows in the column */
                int b0 = (pat >> r) & 1;
                int b1 = (pat >> (r + 1)) & 1;
                int b2 = (pat >> (r + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 0)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) allowed[allowed_cnt++] = pat;
        }

        /* 2. precompute transitions between states
           state = (prev2 << PATTERN_BITS) | prev1  */
        for (int s = 0; s < STATE_COUNT; ++s) {
            int prev2 = s >> PATTERN_BITS;
            int prev1 = s & ((1 << PATTERN_BITS) - 1);
            int cnt = 0;
            for (int k = 0; k < allowed_cnt; ++k) {
                int pat = allowed[k];
                int good = 1;
                for (int r = 0; r < PATTERN_BITS; ++r) {
                    int b2 = (prev2 >> r) & 1;
                    int b1 = (prev1 >> r) & 1;
                    int b0 = (pat    >> r) & 1;
                    if (b2 == b1 && b1 == b0) { /* horizontal triple forbidden */
                        good = 0;
                        break;
                    }
                }
                if (good) {
                    int ns = (prev1 << PATTERN_BITS) | pat;
                    trans_next[s][cnt++] = (uint16_t)ns;
                }
            }
            trans_cnt[s] = cnt;
        }

        initialised = 1;
    }

    /* ------- compute a(n) on the fly (no memoisation) ------- */

    if (n == 1) {
        seq_t zero = 0;
        return (seq_t)allowed_cnt;
    }

    if (n == 2) {
        seq_t zero = 0;
        seq_t val = (seq_t)allowed_cnt;
        return val * val;
    }

    /* n >= 3: use DP */
    seq_t zero = 0;

    /* initialise dp for length 2 (pairs of columns) */
    zero_seq_array(dp, STATE_COUNT);
    for (int i = 0; i < allowed_cnt; ++i) {
        for (int j = 0; j < allowed_cnt; ++j) {
            int state = (allowed[i] << PATTERN_BITS) | allowed[j];
            dp[state] = dp[state] + 1;
        }
    }

    /* iterate from length 3 up to n */
    for (idx_t cur = 3; cur <= n; ++cur) {
        zero_seq_array(newdp, STATE_COUNT);
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp[s];
            /* Skipping zero check keeps code simple and fast enough */
            int tcnt = trans_cnt[s];
            for (int t = 0; t < tcnt; ++t) {
                int ns = trans_next[s][t];
                newdp[ns] = newdp[ns] + ways;
            }
        }
        /* copy newdp into dp for next iteration */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = newdp[s];
    }

    /* sum all states for the final length */
    seq_t sum = 0;
    for (int s = 0; s < STATE_COUNT; ++s) sum = sum + dp[s];

    return sum;
}
