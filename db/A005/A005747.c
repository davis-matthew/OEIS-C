/*
 * A005747  Number of n-covers of an unlabeled 6-set.
 * https://oeis.org/A005747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B6(n) - B5(n), where Bk(n) = number of n X k binary
 *            matrices up to row and column permutations (zero rows/columns
 *            allowed). By Burnside, Bk(n) = (1/k!) * Sum_{sigma in S_k} [x^n]
 *            Prod_{orbits O of sigma on subsets of [k]} 1/(1 - x^|O|).
 *            Removing the zero columns shows that matrices with al...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 25, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 910 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static void add3(uint64_t *a, const uint64_t *b) {
    unsigned __int128 c = 0;
    int j;
    for (j = 0; j < 3; j++) {
        c += (unsigned __int128)a[j] + b[j];
        a[j] = (uint64_t)c;
        c >>= 64;
    }
}

static void addmul3(uint64_t *a, const uint64_t *b, uint64_t m) {
    unsigned __int128 carry = 0;
    int j;
    for (j = 0; j < 3; j++) {
        unsigned __int128 p = (unsigned __int128)b[j] * m + a[j] + carry;
        a[j] = (uint64_t)p;
        carry = p >> 64;
    }
}

static void gen_classes(int k, int rem, int mx, int *cur, int len,
                        int (*perm)[6], long *cnt, int *np) {
    if (rem == 0) {
        int pos = 0, mult[7], i, j;
        long d = 1, f = 1;
        memset(mult, 0, sizeof(mult));
        for (i = 0; i < len; i++) {
            int c = cur[i];
            for (j = 0; j < c; j++) perm[*np][pos + j] = pos + (j + 1) % c;
            pos += c;
            d *= c;
            mult[c]++;
        }
        for (i = 1; i <= k; i++)
            for (j = 2; j <= mult[i]; j++) d *= j;
        for (i = 2; i <= k; i++) f *= i;
        cnt[*np] = f / d;
        (*np)++;
        return;
    }
    {
        int i;
        for (i = (mx < rem ? mx : rem); i >= 1; i--) {
            cur[len] = i;
            gen_classes(k, rem - i, i, cur, len + 1, perm, cnt, np);
        }
    }
}

static int orbit_lens(int k, const int *p, int *lens) {
    char vis[64];
    int nm = 1 << k, no = 0, m;
    memset(vis, 0, sizeof(vis));
    for (m = 0; m < nm; m++) {
        if (!vis[m]) {
            int L = 0, x = m;
            do {
                int y = 0, i;
                vis[x] = 1;
                L++;
                for (i = 0; i < k; i++)
                    if ((x >> i) & 1) y |= 1 << p[i];
                x = y;
            } while (x != m);
            lens[no++] = L;
        }
    }
    return no;
}

/* tot: 3*(N+1) limbs, accumulates Sum_sigma series mod 2^192 */
static void build_fixed(int k, idx_t N, uint64_t *tot) {
    int perm[16][6], cur[8], np = 0, c, lens[64];
    long cnt[16];
    uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * 3 * (size_t)(N + 1));
    gen_classes(k, k, k, cur, 0, perm, cnt, &np);
    memset(tot, 0, sizeof(uint64_t) * 3 * (size_t)(N + 1));
    for (c = 0; c < np; c++) {
        int no = orbit_lens(k, perm[c], lens), o;
        idx_t i;
        memset(t, 0, sizeof(uint64_t) * 3 * (size_t)(N + 1));
        t[0] = 1;
        for (o = 0; o < no; o++) {
            idx_t L = (idx_t)lens[o];
            for (i = L; i <= N; i++) add3(t + 3 * i, t + 3 * (i - L));
        }
        for (i = 0; i <= N; i++) addmul3(tot + 3 * i, t + 3 * i, (uint64_t)cnt[c]);
    }
    free(t);
}

static void build_big(int k, idx_t N, seq_t *tot) {
    int perm[16][6], cur[8], np = 0, c, lens[64];
    long cnt[16];
    idx_t i;
    seq_t *t = alloc_zero(N + 1);
    gen_classes(k, k, k, cur, 0, perm, cnt, &np);
    for (i = 0; i <= N; i++) tot[i] = 0;
    for (c = 0; c < np; c++) {
        int no = orbit_lens(k, perm[c], lens), o;
        for (i = 0; i <= N; i++) t[i] = 0;
        t[0] = 1;
        for (o = 0; o < no; o++) {
            idx_t L = (idx_t)lens[o];
            for (i = L; i <= N; i++) t[i] += t[i - L];
        }
        for (i = 0; i <= N; i++) tot[i] += t[i] * (seq_t)cnt[c];
    }
    release(t, N + 1);
}

static unsigned __int128 inv_odd(unsigned __int128 a) {
    unsigned __int128 x = a;
    int i;
    for (i = 0; i < 8; i++) x *= (unsigned __int128)2 - a * x;
    return x;
}

/* exact division of a 192-bit residue by 2^s * odd, result mod 2^128 */
static unsigned __int128 div_exact(const uint64_t *l, int s, unsigned __int128 odd) {
    uint64_t lo = (l[0] >> s) | (l[1] << (64 - s));
    uint64_t hi = (l[1] >> s) | (l[2] << (64 - s));
    unsigned __int128 v = ((unsigned __int128)hi << 64) | lo;
    return v * inv_odd(odd);
}

static idx_t have = 0;
static uint64_t *f6 = NULL, *f5 = NULL;
static seq_t *b6 = NULL, *b5 = NULL;

seq_t a(idx_t n) {
    int big = seq_is_big();
    if (n > have) {
        idx_t N = n;
        if (N < 2 * have) N = 2 * have;
        if (N < 64) N = 64;
        if (big) {
            if (b6) release(b6, have + 1);
            if (b5) release(b5, have + 1);
            b6 = alloc_zero(N + 1);
            b5 = alloc_zero(N + 1);
            build_big(6, N, b6);
            build_big(5, N, b5);
        } else {
            free(f6);
            free(f5);
            f6 = (uint64_t *)malloc(sizeof(uint64_t) * 3 * (size_t)(N + 1));
            f5 = (uint64_t *)malloc(sizeof(uint64_t) * 3 * (size_t)(N + 1));
            build_fixed(6, N, f6);
            build_fixed(5, N, f5);
        }
        have = N;
    }
    if (big) {
        return b6[n] / 720 - b5[n] / 120;
    } else {
        unsigned __int128 a6 = div_exact(f6 + 3 * n, 4, 45);
        unsigned __int128 a5 = div_exact(f5 + 3 * n, 3, 15);
        return u128_to_seq(a6 - a5);
    }
}
