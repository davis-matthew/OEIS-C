/*
 * A213116  Number of binary arrays of length n+11 with fewer than 6 ones in
 *          any length 12 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary arrays of length n+11 that have fewer
 *            than 6 ones in every consecutive block of 12 bits (i.e., weight
 *            of any length‑12 subsequence ≤5)
 * Range:     u8 none, u16 n <= 7, u32 n <= 27, u64 n <= 66, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MASK 0x7FFU   /* 11 bits */

/* Helper: precomputed popcount for 11‑bit masks */
static unsigned char popcnt[1 << 11];
static int popcnt_inited = 0;

static void init_popcnt(void)
{
    if (popcnt_inited) return;
    popcnt[0] = 0;
    for (int i = 1; i < (1 << 11); ++i)
        popcnt[i] = popcnt[i >> 1] + (i & 1);
    popcnt_inited = 1;
}

seq_t a(idx_t n)
{
    init_popcnt();

    /* Two DP arrays of size 2048 */
    seq_t dp0[1 << 11] = { 0 };
    seq_t dp1[1 << 11] = { 0 };

    seq_t *dp   = dp0;
    seq_t *next = dp1;

    dp[0] = 1;                     /* empty string */
    seq_t L = (seq_t)n + 11;       /* length of strings to be counted */

    for (idx_t i = 0; i < L; ++i) {
        /* clear next array */
        memset(next, 0, sizeof(next[0]) * (1 << 11));

        int need_constraint = (i >= 11);   /* only after first 11 bits */

        for (int mask = 0; mask < (1 << 11); ++mask) {
            seq_t cnt = dp[mask];
            if (!cnt) continue;
            for (int bit = 0; bit < 2; ++bit) {
                if (need_constraint) {
                    if (popcnt[mask] + bit > 5) continue;
                }
                int newmask = ((mask << 1) | bit) & MASK;
                next[newmask] += cnt;
            }
        }

        /* swap dp and next arrays */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* sum all reachable suffixes */
    seq_t result = 0;
    for (int mask = 0; mask < (1 << 11); ++mask)
        result += dp[mask];

    return result;
}
