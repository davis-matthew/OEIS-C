/*
 * A215245  a(n) = minimal value of A215244(k) for 2^n <= k < 2^(n+1).
 * https://oeis.org/A215245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = min{ A215244(k) : 2^n <= k < 2^(n+1) }, where A215244(k)
 *            is the number of ways to write the binary expansion of k as a
 *            concatenation of palindromes. Computed by a DFS over all
 *            (n+1)-bit strings, with the DP and the set of palindromic
 *            suffixes updated incrementally
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A215245_dfs(int L, uint64_t M, int total, unsigned char *bits,
                        seq_t *dp, seq_t *best, bool *have)
{
    if (L == total) {
        if (!*have || dp[L] < *best) {
            *best = dp[L];
            *have = true;
        }
        return;
    }
    for (int b = 0; b < 2; ++b) {
        uint64_t nm = ((uint64_t)1 << L);
        if (bits[L - 1] == b) nm |= ((uint64_t)1 << (L - 1));
        uint64_t t = M & ~(uint64_t)1; /* starts j1 >= 1 */
        while (t) {
#ifdef __GNUC__
            int j1 = __builtin_ctzll(t);
#else
            int j1 = 0;
            while (!((t >> j1) & 1)) ++j1;
#endif
            t &= t - 1;
            if (bits[j1 - 1] == b) nm |= ((uint64_t)1 << (j1 - 1));
        }
        seq_t s = 0;
        uint64_t u = nm;
        while (u) {
#ifdef __GNUC__
            int j = __builtin_ctzll(u);
#else
            int j = 0;
            while (!((u >> j) & 1)) ++j;
#endif
            u &= u - 1;
            s += dp[j];
        }
        bits[L] = (unsigned char)b;
        dp[L + 1] = s;
        A215245_dfs(L + 1, nm, total, bits, dp, best, have);
    }
}

seq_t a(idx_t n)
{
    int total = (int)n + 1;
    unsigned char bits[64];
    seq_t *dp = mem_new(seq_t, 66);
    seq_t best = 0;
    bool have = false;
    bits[0] = 1;
    dp[0] = 1;
    dp[1] = 1;
    A215245_dfs(1, 1, total, bits, dp, &best, &have);
    seq_t r = best;
    mem_free(dp);
    return r;
}
