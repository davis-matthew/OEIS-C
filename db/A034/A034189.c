/*
 * A034189  Number of binary codes of length 4 with n words.
 * https://oeis.org/A034189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of orbits of n‑subsets of the vertices of the
 *            4‑cube * under the full automorphism group (coordinate
 *            permutations and bit * inversions). Computed by Burnside's lemma
 *            – no hard‑coded table
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   apply_elem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/apply_elem.h"

/* list of all 24 permutations of the 4 coordinates */
static const int perms[24][4] = {
    {0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1},
    {1,0,2,3},{1,0,3,2},{1,2,0,3},{1,2,3,0},{1,3,0,2},{1,3,2,0},
    {2,0,1,3},{2,0,3,1},{2,1,0,3},{2,1,3,0},{2,3,0,1},{2,3,1,0},
    {3,0,1,2},{3,0,2,1},{3,1,0,2},{3,1,2,0},{3,2,0,1},{3,2,1,0}
};

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(n) = 0 for n > 16 */
    if (n > 16) {
        return (seq_t)0;
    }

    /* memoization */
    static seq_t values[17] = {0};
    static bool initialized = false;
    if (!initialized) {
        static seq_t sums[17] = {0};
        bool visited[16];
        seq_t dp[17];
        unsigned int group_size = 384u; /* 24 permutations * 16 bit masks */

        /* iterate over all group elements */
        for (const int (*p)[4] = perms; p < perms + 24; ++p) {
            for (int b = 0; b < 16; ++b) {
                /* cycle decomposition */
                for (int i = 0; i < 16; ++i) visited[i] = false;
                int cycle_len[16];
                int cycle_cnt = 0;

                for (int v = 0; v < 16; ++v) {
                    if (!visited[v]) {
                        int len = 0;
                        int cur = v;
                        while (!visited[cur]) {
                            visited[cur] = true;
                            cur = apply_elem(cur, b, *p);
                            ++len;
                        }
                        cycle_len[cycle_cnt++] = len;
                    }
                }

                /* DP for subset sums of cycle lengths */
                for (int i = 0; i <= 16; ++i) dp[i] = (seq_t)0;
                dp[0] = (seq_t)1;
                for (int i = 0; i < cycle_cnt; ++i) {
                    int L = cycle_len[i];
                    for (int j = 16; j >= L; --j) {
                        dp[j] += dp[j - L];
                    }
                }

                /* accumulate counts */
                for (int i = 0; i <= 16; ++i) {
                    sums[i] += dp[i];
                }
            }
        }

        /* divide by |G|=384 */
        seq_t denom = (seq_t)group_size;
        for (int i = 0; i <= 16; ++i) {
            values[i] = sums[i] / denom;
        }

        initialized = true;
    }
    return values[n];
}
