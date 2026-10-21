/*
 * A206933  Number of nX5 0..1 arrays avoiding 0 0 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding 000 horizontally and
 *            011 vertically
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int allowed[24];
static int allowedCount;
static unsigned char transCnt[576];
static unsigned short transNext[576][24];
static int tables_built = 0;

static void build_tables(void)
{
    /* Build allowed masks (5 bits) without horizontal 000 */
    allowedCount = 0;
    for (int mask = 0; mask < 32; ++mask) {
        int ok = 1;
        for (int j = 0; j <= 2; ++j) {
            if ((mask & (7 << j)) == 0) {   /* three consecutive zeros */
                ok = 0;
                break;
            }
        }
        if (ok) {
            allowed[allowedCount++] = mask;
        }
    }

    /* Build transition table: from (prevprev, prev) -> next states */
    int states = allowedCount * allowedCount;
    for (int i = 0; i < states; ++i) transCnt[i] = 0;

    for (int a = 0; a < allowedCount; ++a) {
        for (int b = 0; b < allowedCount; ++b) {
            int stateIdx = a * allowedCount + b;
            for (int c = 0; c < allowedCount; ++c) {
                int maskA = allowed[a];
                int maskB = allowed[b];
                int maskC = allowed[c];
                int valid = 1;
                for (int pos = 0; pos < 5; ++pos) {
                    if (((maskA >> pos) & 1) == 0 &&
                        ((maskB >> pos) & 1) == 1 &&
                        ((maskC >> pos) & 1) == 1) {
                        valid = 0;
                        break;
                    }
                }
                if (valid) {
                    int nextState = b * allowedCount + c;
                    transNext[stateIdx][transCnt[stateIdx]++] = nextState;
                }
            }
        }
    }
    tables_built = 1;
}

/* DP arrays: enough for max 24*24 = 576 states */
static seq_t dp[576];
static seq_t next_arr[576];

seq_t a(idx_t n)
{
    if (!tables_built) build_tables();

    if (n == 1) return (seq_t)allowedCount;  /* a(1) = 24 */

    int states = allowedCount * allowedCount;

    /* Initialize dp with all 1's for pairs of rows */
    for (int i = 0; i < states; ++i) dp[i] = (seq_t)1;

    /* Iterate over remaining rows (3 to n) */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < states; ++i) next_arr[i] = (seq_t)0;
        for (int state = 0; state < states; ++state) {
            seq_t val = dp[state];
            if (val == (seq_t)0) continue;
            unsigned char m = transCnt[state];
            for (unsigned char k = 0; k < m; ++k) {
                int ns = transNext[state][k];
                next_arr[ns] += val;
            }
        }
        for (int i = 0; i < states; ++i) dp[i] = next_arr[i];
    }

    /* Sum all states to obtain the total count */
    seq_t result = (seq_t)0;
    for (int i = 0; i < states; ++i) result += dp[i];
    return result;
}
