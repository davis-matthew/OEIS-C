/*
 * A004888  Numbers that are the sum of at most 4 positive 9th powers.
 * https://oeis.org/A004888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: list every sum p1^9+p2^9+p3^9+p4^9 with 0 <= p1 <= p2 <=
 *            p3 <= p4 <= K that is below (K+1)^9, then sort and remove
 *            duplicates; K grows until there are at least n terms
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *terms = NULL;
static idx_t nterms = 0;

static void build(idx_t K) {
    uint64_t pw[128];
    idx_t i, j, k, l, cap = 1024, len = 0;
    uint64_t L, *buf;
    for (i = 0; i <= K + 1; i++) {
        uint64_t p = 1;
        int e;
        for (e = 0; e < 9; e++) p *= (uint64_t)i;
        pw[i] = p;
    }
    L = pw[K + 1];
    buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 0; i <= K; i++) {
        if (pw[i] >= L) break;
        for (j = i; j <= K; j++) {
            uint64_t s2 = pw[i] + pw[j];
            if (s2 >= L) break;
            for (k = j; k <= K; k++) {
                uint64_t s3 = s2 + pw[k];
                if (s3 >= L) break;
                for (l = k; l <= K; l++) {
                    uint64_t s4 = s3 + pw[l];
                    if (s4 >= L) break;
                    if (len == cap) {
                        cap *= 2;
                        buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                    }
                    buf[len++] = s4;
                }
            }
        }
    }
    qsort(buf, len, sizeof(uint64_t), cmp_u64);
    {
        idx_t u = 0;
        for (i = 0; i < len; i++)
            if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
        len = u;
    }
    free(terms);
    terms = buf;
    nterms = len;
}

seq_t a(idx_t n) {
    static idx_t K = 0;
    if (K == 0) {
        K = 8;
        build(K);
    }
    while (nterms < n && K < 100) {
        K = K + K / 2 + 1;
        if (K > 100) K = 100;
        build(K);
    }
    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
