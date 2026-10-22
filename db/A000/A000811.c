/*
 * A000811  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n!) * Sum_{g in S_n} 4^{c(g)}, where c(g) is the
 *            number of cycles of g acting on the 2^n input vectors. The sum
 *            is taken over the cycle types of g (partitions of n), weighted
 *            by n!/z_lambda; 2^v2(n!) is divided out of each term, then the
 *            odd part of n! is divided out exactly
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   is_fixed_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_fixed_seq.h"

static unsigned char *vis;
static int perm[32];
static int parts[32];
static int N;
static uint64_t nfact;
static uint64_t e2;
static seq_t acc;

static seq_t pw2(uint64_t k) {
    seq_t result = 1;
    seq_t base = 2;
    while (k > 0) {
        if (k & 1) result *= base;
        k >>= 1;
        if (k) base *= base;
    }
    return result;
}

static uint64_t count_cycles(void) {
    uint64_t total = (uint64_t)1 << N, c = 0, x;
    memset(vis, 0, (size_t)total);
    for (x = 0; x < total; x++) {
        uint64_t y;
        if (vis[x]) continue;
        c++;
        y = x;
        do {
            uint64_t img = 0;
            int i;
            vis[y] = 1;
            for (i = 0; i < N; i++)
                if (y & ((uint64_t)1 << i)) img |= (uint64_t)1 << perm[i];
            y = img;
        } while (y != x);
    }
    return c;
}

static void rec(int rem, int maxp, int k) {
    if (rem == 0) {
        int pos = 0, i, j;
        uint64_t z = 1, c, cnt;
        for (i = 0; i < k; i++) {
            int L = parts[i];
            for (j = 0; j < L; j++) perm[pos + j] = pos + (j + 1) % L;
            pos += L;
            z *= (uint64_t)L;
        }
        i = 0;
        while (i < k) {
            int r = i;
            uint64_t m = 0;
            while (r < k && parts[r] == parts[i]) {
                r++;
                m++;
                z *= m;
            }
            i = r;
        }
        cnt = nfact / z;
        c = count_cycles();
        acc += (seq_t)cnt * pw2(2 * c - e2);
        return;
    }
    {
        int p = rem < maxp ? rem : maxp;
        for (; p >= 1; p--) {
            parts[k] = p;
            rec(rem - p, p, k + 1);
        }
    }
}

seq_t a(idx_t n) {
    uint64_t odd, t, i;
    seq_t res;
    if (n == 0 || n > 20) return 0;
    N = (int)n;
    nfact = 1;
    for (i = 1; i <= n; i++) nfact *= i;
    e2 = 0;
    t = nfact;
    while ((t & 1) == 0) {
        t >>= 1;
        e2++;
    }
    odd = t;
    vis = (unsigned char *)malloc((size_t)((uint64_t)1 << N));
    acc = 0;
    rec(N, N, 0);
    free(vis);
    if (is_fixed_seq()) {
        seq_t o = (seq_t)odd, inv = (seq_t)odd;
        int k;
        for (k = 0; k < 8; k++) inv *= (seq_t)2 - o * inv;
        res = acc * inv;
    } else {
        res = acc / (seq_t)odd;
    }
    return res;
}
