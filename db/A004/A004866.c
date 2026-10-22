/*
 * A004866  Numbers that are the sum of at most 4 positive 7th powers.
 * https://oeis.org/A004866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most four 7th powers below a
 *            bound (k^7 for parts <= k-1), sort, dedupe, and grow the bound
 *            until n terms are found
 * Range:     u8 n <= 9, u16 n <= 69, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static int cmp_u64(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *terms = NULL;
static idx_t nterms = 0;

static void build(uint64_t B)
{
    uint64_t p[1024];
    uint64_t L, i, j, k, l;
    size_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 0; i <= B + 1; i++) {
        uint64_t t = 1;
        int e;
        for (e = 0; e < 7; e++) t *= i;
        p[i] = t;
    }
    L = p[B + 1];
    for (i = 0; i <= B; i++) {
        uint64_t si = p[i];
        if (si >= L) break;
        for (j = i; j <= B; j++) {
            uint64_t sj = si + p[j];
            if (sj >= L) break;
            for (k = j; k <= B; k++) {
                uint64_t sk = sj + p[k];
                if (sk >= L) break;
                for (l = k; l <= B; l++) {
                    uint64_t sl = sk + p[l];
                    if (sl >= L) break;
                    if (cnt == cap) {
                        cap *= 2;
                        v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                    }
                    v[cnt++] = sl;
                }
            }
        }
    }
    qsort(v, cnt, sizeof(uint64_t), cmp_u64);
    {
        size_t u = 0, t;
        for (t = 0; t < cnt; t++)
            if (t == 0 || v[t] != v[t - 1]) v[u++] = v[t];
        cnt = u;
    }
    free(terms);
    terms = v;
    nterms = (idx_t)cnt;
}

seq_t a(idx_t n)
{
    static uint64_t B = 0;
    if (B == 0) {
        B = 8;
        build(B);
    }
    while (nterms < n && B < 500) {
        B *= 2;
        if (B > 500) B = 500;
        build(B);
    }
    if (n < 1 || n > nterms) return 0;
    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
