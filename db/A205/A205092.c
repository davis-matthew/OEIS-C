/*
 * A205092  Number of (n+1) X 4 0..3 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A205092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} in which all *
 *            2×2 subblocks have the same number of horizontal and vertical
 *            edge * increases. The value is computed by dynamic programming
 *            over row * transitions for each possible common count k ∈
 *            {0,…,4}. For a fixed * k, rows s→t are adjacent iff...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


static uint8_t digits[256][4];
static uint8_t *nexts[5][256];
static size_t next_len[5][256];
static bool initialized = false;

static bool valid_pair_k(int k, uint8_t s, uint8_t t)
{
    const uint8_t *A = digits[s];
    const uint8_t *B = digits[t];
    int inc0 = (A[1] > A[0]) + (B[1] > B[0]) +
               (B[0] > A[0]) + (B[1] > A[1]);
    int inc1 = (A[2] > A[1]) + (B[2] > B[1]) +
               (B[1] > A[1]) + (B[2] > A[2]);
    int inc2 = (A[3] > A[2]) + (B[3] > B[2]) +
               (B[2] > A[2]) + (B[3] > A[3]);
    return inc0 == inc1 && inc1 == inc2 && inc0 == k;
}

static void init(void)
{
    if (initialized)
        return;

    /* populate digits table */
    for (uint16_t s = 0; s < 256; ++s) {
        digits[s][0] = (s >> 6) & 3;
        digits[s][1] = (s >> 4) & 3;
        digits[s][2] = (s >> 2) & 3;
        digits[s][3] = s & 3;
    }

    /* build adjacency lists for each k */
    for (int k = 0; k <= 4; ++k) {
        for (uint16_t s = 0; s < 256; ++s) {
            size_t cnt = 0;
            for (uint16_t t = 0; t < 256; ++t)
                if (valid_pair_k(k, (uint8_t)s, (uint8_t)t))
                    ++cnt;
            next_len[k][s] = cnt;
            if (cnt > 0)
                nexts[k][s] = (uint8_t *)malloc(cnt);
            else
                nexts[k][s] = NULL;
        }
        /* store successors */
        for (uint16_t s = 0; s < 256; ++s) {
            size_t pos = 0;
            for (uint16_t t = 0; t < 256; ++t)
                if (valid_pair_k(k, (uint8_t)s, (uint8_t)t))
                    nexts[k][s][pos++] = (uint8_t)t;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init();                     /* ensure precomputation is done */
    if (n == 0)                 /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* two DP layers; dimensions: [k][state] */
    seq_t dp[2][5][256];
    int cur = 0, nxt = 1;

    /* initial row: any of the 256 states is possible */
    for (int k = 0; k <= 4; ++k)
        for (int s = 0; s < 256; ++s)
            dp[cur][k][s] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* zero next layer */
        for (int k = 0; k <= 4; ++k)
            for (int s = 0; s < 256; ++s)
                dp[nxt][k][s] = (seq_t)0;

        /* transition */
        for (int k = 0; k <= 4; ++k) {
            for (int s = 0; s < 256; ++s) {
                seq_t val = dp[cur][k][s];
                if (val == (seq_t)0)
                    continue;
                size_t len = next_len[k][s];
                if (len == 0)
                    continue;
                uint8_t *succ = nexts[k][s];
                for (size_t i = 0; i < len; ++i)
                    dp[nxt][k][succ[i]] += val;
            }
        }
        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all k and states */
    seq_t result = (seq_t)0;
    for (int k = 0; k <= 4; ++k)
        for (int s = 0; s < 256; ++s)
            result += dp[cur][k][s];

    return result;
}
