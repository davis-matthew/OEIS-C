/*
 * A005162  Number of alternating sign n X n matrices symmetric with respect
 *          to both diagonals.
 * https://oeis.org/A005162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   transfer-matrix count of the corner-sum height functions h(i,j)
 *            (0<=i,j<=n, h(0,j)=j, neighbouring values differ by 1) that
 *            satisfy h(i,j)=h(j,i)=h(n-j,n-i). Only the fundamental triangle
 *            i<=j, i+j<=n is enumerated, one row at a time. With e(i,j) =
 *            (h(i,j)-j+i)/2, row 0 is all 0; then e(i,j) is e...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static uint64_t *hk;      /* hash keys */
static long long *hv;     /* hash -> index, -1 empty */
static size_t hcap;
static seq_t *cnt;
static uint64_t *okeys;
static idx_t ncnt, capcnt;

static size_t hslot(uint64_t key)
{
    uint64_t h = key * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(hcap - 1));
}

static void hrebuild(size_t newcap)
{
    size_t k;
    idx_t t;
    free(hk);
    free(hv);
    hcap = newcap;
    hk = (uint64_t *)malloc(hcap * sizeof(uint64_t));
    hv = (long long *)malloc(hcap * sizeof(long long));
    for (k = 0; k < hcap; k++) hv[k] = -1;
    for (t = 0; t < ncnt; t++) {
        size_t s = hslot(okeys[t]);
        while (hv[s] >= 0) s = (s + 1) & (hcap - 1);
        hk[s] = okeys[t];
        hv[s] = (long long)t;
    }
}

static void hadd(uint64_t key, const seq_t *c)
{
    size_t s = hslot(key);
    while (hv[s] >= 0) {
        if (hk[s] == key) {
            cnt[hv[s]] += *c;
            return;
        }
        s = (s + 1) & (hcap - 1);
    }
    if (ncnt == capcnt) {
        idx_t nc = capcnt * 2;
        cnt = grow_zero(cnt, capcnt, nc);
        okeys = (uint64_t *)realloc(okeys, (size_t)nc * sizeof(uint64_t));
        capcnt = nc;
    }
    hk[s] = key;
    hv[s] = (long long)ncnt;
    okeys[ncnt] = key;
    cnt[ncnt] = *c;
    ncnt++;
    if ((size_t)ncnt * 2 > hcap) hrebuild(hcap * 2);
}

static void gen(const int *p, int *e, int t, int L, const seq_t *c)
{
    int lo, hi, v;
    if (t == L) {
        uint64_t mask = 0;
        int k;
        for (k = 0; k + 1 < L; k++)
            if (e[k] != e[k + 1]) mask |= (uint64_t)1 << k;
        hadd((mask << 8) | (uint64_t)e[0], c);
        return;
    }
    lo = p[t + 1];
    hi = p[t + 1] + 1;
    if (t > 0) {
        if (lo < e[t - 1] - 1) lo = e[t - 1] - 1;
        if (hi > e[t - 1]) hi = e[t - 1];
    }
    for (v = lo; v <= hi; v++) {
        e[t] = v;
        gen(p, e, t + 1, L, c);
    }
}

seq_t a(idx_t n)
{
    idx_t m, i, k;
    uint64_t *pk;
    seq_t *pc;
    idx_t pn, pcap;
    int *p, *e;
    seq_t r;

    if (n == 0) return (seq_t)0;
    m = n / 2;
    p = (int *)malloc((size_t)(n + 3) * sizeof(int));
    e = (int *)malloc((size_t)(n + 3) * sizeof(int));
    pk = (uint64_t *)malloc(sizeof(uint64_t));
    pk[0] = 0;
    pc = alloc_zero(1);
    pc[0] = 1;
    pn = 1;
    pcap = 1;

    for (i = 1; i <= m; i++) {
        int L = (int)(n - 2 * i + 1);
        int Lp = L + 2;
        hk = NULL;
        hv = NULL;
        ncnt = 0;
        capcnt = 16;
        cnt = alloc_zero(capcnt);
        okeys = (uint64_t *)malloc((size_t)capcnt * sizeof(uint64_t));
        hrebuild(64);
        for (k = 0; k < pn; k++) {
            uint64_t key = pk[k];
            uint64_t mask = key >> 8;
            int t;
            p[0] = (int)(key & 0xFF);
            for (t = 0; t + 1 < Lp; t++)
                p[t + 1] = p[t] - (int)((mask >> t) & 1);
            gen(p, e, 0, L, &pc[k]);
        }
        release(pc, pcap);
        free(pk);
        pk = okeys;
        pc = cnt;
        pn = ncnt;
        pcap = capcnt;
        free(hk);
        free(hv);
        hk = NULL;
        hv = NULL;
    }

    r = 0;
    for (k = 0; k < pn; k++) r += pc[k];
    release(pc, pcap);
    free(pk);
    free(p);
    free(e);
    return r;
}
