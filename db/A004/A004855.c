/*
 * A004855  Numbers that are the sum of at most 4 nonzero 6th powers.
 * https://oeis.org/A004855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums i^6+j^6+k^6+l^6 with 0<=i<=j<=k<=l
 *            up to a bound X, sort, dedupe; double X until at least n terms
 *            are found
 * Range:     u8 n <= 14, u16 n <= 171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static int cmp_u64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void build(uint64_t X) {
    uint64_t p[2000];
    int m = 0;
    while (1) {
        uint64_t i = (uint64_t)m;
        uint64_t v = i * i * i * i * i * i;
        if (v > X || m >= 1999) break;
        p[m++] = v;
    }
    size_t cap = 1024, cnt = 0;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (int i = 0; i < m; i++) {
        uint64_t si = p[i];
        if (si > X) break;
        for (int j = i; j < m; j++) {
            uint64_t sj = si + p[j];
            if (sj > X) break;
            for (int k = j; k < m; k++) {
                uint64_t sk = sj + p[k];
                if (sk > X) break;
                for (int l = k; l < m; l++) {
                    uint64_t sl = sk + p[l];
                    if (sl > X) break;
                    if (cnt == cap) {
                        cap *= 2;
                        buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                    }
                    buf[cnt++] = sl;
                }
            }
        }
    }
    qsort(buf, cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t t = 0; t < cnt; t++) {
        if (u == 0 || buf[t] != buf[u - 1]) buf[u++] = buf[t];
    }
    free(cache);
    cache = buf;
    cache_len = (idx_t)u;
}

seq_t a(idx_t n) {
    uint64_t X = (uint64_t)1 << 20;
    while (cache == NULL || cache_len < n) {
        build(X);
        X *= 2;
    }
    return (seq_t)cache[n - 1];
}
