/*
 * A000348  Number of ways to pair up {1^2, 2^2, ..., (2n)^2 } so sum of each
 *          pair is prime.
 * https://oeis.org/A000348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = permanent(m), m(i,j) = [ (2i)^2+(2j-1)^2 is prime ].
 *            Computed by counting perfect matchings: entries vanish mod 5
 *            unless the residues of (2i)^2 and (2j-1)^2 mod 5 are compatible
 *            (the single exception 2^2+1^2 = 5 is handled by expanding along
 *            that entry), which splits rows and columns into...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   a348_dp, a348_is_prime, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a348_dp.h"
#include "../utils/a348_is_prime.h"

/* context for the enumeration of class-0 rows */
static int a348_r0, a348_r1, a348_r4, a348_c0, a348_c1, a348_c4;
static uint64_t a348_adjZ[64], a348_adjW[64];
static uint64_t *a348_G, *a348_H;
static unsigned __int128 a348_acc;

static void a348_dfs(int t, uint64_t Z, uint64_t W) {
    if (t == a348_r0) {
        uint64_t fullC0 = (a348_c0 == 64) ? ~0ULL : ((1ULL << a348_c0) - 1);
        uint64_t zc = ((1ULL << a348_c1) - 1) & ~Z;
        uint64_t wc = ((1ULL << a348_c4) - 1) & ~W;
        int pz = __builtin_popcountll(zc), pw = __builtin_popcountll(wc);
        for (uint64_t V = 0; V <= fullC0; V++) {
            int pv = __builtin_popcountll(V);
            if (pv + pz == a348_r1 && (a348_c0 - pv) + pw == a348_r4) {
                uint64_t gi = V | (zc << a348_c0);
                uint64_t hi = (fullC0 & ~V) | (wc << a348_c0);
                a348_acc += (unsigned __int128)a348_G[gi] * a348_H[hi];
            }
            if (V == fullC0) break;
        }
        return;
    }
    uint64_t av = a348_adjZ[t] & ~Z;
    while (av) {
        uint64_t b = av & (~av + 1);
        av &= av - 1;
        a348_dfs(t + 1, Z | b, W);
    }
    av = a348_adjW[t] & ~W;
    while (av) {
        uint64_t b = av & (~av + 1);
        av &= av - 1;
        a348_dfs(t + 1, Z, W | b);
    }
}

/* number of perfect matchings, ignoring the entry (row e=2, col o=1);
   if skip, row e=2 and column o=1 are removed */
static unsigned __int128 a348_count(int n, int skip) {
    uint64_t R0[64], R1[64], R4[64], C0[64], C1[64], C4[64];
    int r0 = 0, r1 = 0, r4 = 0, c0 = 0, c1 = 0, c4 = 0;
    for (int i = 1; i <= n; i++) {
        if (skip && i == 1) continue;
        uint64_t e = (uint64_t)(2 * i);
        int cl = (int)((e * e) % 5);
        if (cl == 0) R0[r0++] = e; else if (cl == 1) R1[r1++] = e; else R4[r4++] = e;
    }
    for (int j = 1; j <= n; j++) {
        if (skip && j == 1) continue;
        uint64_t o = (uint64_t)(2 * j - 1);
        int cl = (int)((o * o) % 5);
        if (cl == 0) C0[c0++] = o; else if (cl == 1) C1[c1++] = o; else C4[c4++] = o;
    }
    if (c0 + c1 > 36 || c0 + c4 > 36) return 0;
    a348_r0 = r0; a348_r1 = r1; a348_r4 = r4;
    a348_c0 = c0; a348_c1 = c1; a348_c4 = c4;
    for (int t = 0; t < r0; t++) {
        uint64_t e = R0[t], mz = 0, mw = 0;
        for (int c = 0; c < c1; c++)
            if (a348_is_prime(e * e + C1[c] * C1[c])) mz |= 1ULL << c;
        for (int c = 0; c < c4; c++)
            if (a348_is_prime(e * e + C4[c] * C4[c])) mw |= 1ULL << c;
        a348_adjZ[t] = mz;
        a348_adjW[t] = mw;
    }
    a348_G = a348_dp(r1, R1, c0, C0, c1, C1);
    a348_H = a348_dp(r4, R4, c0, C0, c4, C4);
    a348_acc = 0;
    a348_dfs(0, 0, 0);
    free(a348_G);
    free(a348_H);
    return a348_acc;
}

static seq_t a348_memo[64];
static bool a348_have[64];

seq_t a(idx_t n) {
    if (n < 1 || n >= 64) return (seq_t)0;
    int N = (int)n;
    if (a348_have[N]) return a348_memo[N];
    unsigned __int128 acc = a348_count(N, 0) + a348_count(N, 1);
    seq_t r = u128_to_seq(acc);
    a348_memo[N] = r;
    a348_have[N] = true;
    return r;
}
