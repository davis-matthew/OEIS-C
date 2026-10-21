/*
 * A235852  Number of (n+1)X(3+1) 0..3 arrays with the upper median equal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A235852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..3 such that in
 *            every 2×2 subblock the two middle order statistics are equal.
 *            Computation via DP over row patterns
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Pre‑computed adjacency lists for the 256 possible 4‑column rows. */
static uint16_t neighbors[256][256];
static uint8_t   neighbor_cnt[256];
static bool      adj_built = false;

/* Check whether the four values a,b,c,d satisfy
   the median condition of a 2×2 block. */
static bool valid_block(int a, int b, int c, int d)
{
    int v[4] = { a, b, c, d };
    /* insertion sort for 4 elements */
    for (int i = 1; i < 4; ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
    return v[1] == v[2];
}

/* Build the adjacency lists once. */
static void build_adj(void)
{
    if (adj_built) return;

    static unsigned char row[256][4];
    for (int s = 0; s < 256; ++s) {
        neighbor_cnt[s] = 0;
        for (int i = 0; i < 4; ++i)
            row[s][i] = (unsigned char)((s >> (2 * (3 - i))) & 0x3);
    }

    for (int s = 0; s < 256; ++s) {
        for (int t = s; t < 256; ++t) {
            bool ok = true;
            for (int i = 0; i < 3 && ok; ++i)
                if (!valid_block(row[s][i], row[s][i + 1], row[t][i], row[t][i + 1]))
                    ok = false;
            if (ok) {
                /* Store adjacency both ways */
                neighbors[s][neighbor_cnt[s]++] = (uint16_t)t;
                if (s != t)
                    neighbors[t][neighbor_cnt[t]++] = (uint16_t)s;
            }
        }
    }
    adj_built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;          /* offset 1 */

    build_adj();

    seq_t dp[256];
    seq_t ndp[256];

    /* Initial row: any of the 256 patterns is allowed. */
    for (int i = 0; i < 256; ++i)
        dp[i] = 1;

    /* Apply n transitions (n additional rows). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i)
            ndp[i] = 0;

        for (int t = 0; t < 256; ++t) {
            seq_t val = dp[t];
            if (val == 0) continue;
            const uint8_t cnt = neighbor_cnt[t];
            const uint16_t *list = neighbors[t];
            for (uint8_t j = 0; j < cnt; ++j)
                ndp[list[j]] += val;
        }

        for (int i = 0; i < 256; ++i)
            dp[i] = ndp[i];
    }

    /* Sum all final state counts. */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += dp[i];

    return total;
}
