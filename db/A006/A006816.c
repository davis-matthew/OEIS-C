/*
 * A006816  Related to self-avoiding walks on square lattice.
 * https://oeis.org/A006816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   exhaustive search. a(n) = sum over all self-avoiding walks of n
 *            steps on the square lattice whose first step is fixed (c(n)/4
 *            walks) of (n + number of non-bonded nearest-neighbor contacts).
 *            The search counts the walks that go straight along the axis
 *            directly, and counts each walk that turns once...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

#define MAXN 48
#define W (2 * MAXN + 3)

static unsigned char vis[W * W];
static unsigned char cnt[W * W];
static const int doff[4] = {1, W, -1, -W};

static void mark(int p) {
    vis[p] = 1;
    cnt[p + 1]++; cnt[p - 1]++; cnt[p + W]++; cnt[p - W]++;
}

static void unmark(int p) {
    vis[p] = 0;
    cnt[p + 1]--; cnt[p - 1]--; cnt[p + W]--; cnt[p - W]--;
}

/* p is the current (already marked) end, rem steps remain, L = links so far */
static uint64_t rec(int p, int rem, uint64_t L) {
    uint64_t s = 0;
    int i, j;
    if (rem == 0) return L;
    if (rem == 1) {
        for (i = 0; i < 4; i++) {
            int q = p + doff[i];
            if (!vis[q]) s += L + cnt[q];
        }
        return s;
    }
    if (rem == 2) {
        for (i = 0; i < 4; i++) {
            int q = p + doff[i];
            uint64_t Lq;
            if (vis[q]) continue;
            Lq = L + cnt[q];
            mark(q);
            for (j = 0; j < 4; j++) {
                int r = q + doff[j];
                if (!vis[r]) s += Lq + cnt[r];
            }
            unmark(q);
        }
        return s;
    }
    for (i = 0; i < 4; i++) {
        int q = p + doff[i];
        if (vis[q]) continue;
        {
            uint64_t Lq = L + cnt[q];
            mark(q);
            s += rec(q, rem - 1, Lq);
            unmark(q);
        }
    }
    return s;
}

static seq_t memo[MAXN + 1];
static bool known[MAXN + 1];

seq_t a(idx_t n) {
    int N, k, origin;
    uint64_t total, turned;
    if (n == 0 || n > MAXN) return 0;
    if (known[n]) return memo[n];
    N = (int)n;
    memset(vis, 0, sizeof(vis));
    memset(cnt, 0, sizeof(cnt));
    origin = (MAXN + 1) * W + (MAXN + 1);
    mark(origin);
    turned = 0;
    /* walk k steps along +x, then turn to +y (mirror image gives factor 2) */
    for (k = 1; k <= N - 1; k++) {
        int pk = origin + k;
        int q = pk + W;
        uint64_t L;
        mark(pk);
        L = (uint64_t)k + cnt[q];
        mark(q);
        turned += rec(q, N - k - 1, L);
        unmark(q);
    }
    total = (uint64_t)N + 2 * turned;
    memo[n] = u128_to_seq((unsigned __int128)total);
    known[n] = true;
    return memo[n];
}
