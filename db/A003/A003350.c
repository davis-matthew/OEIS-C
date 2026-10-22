/*
 * A003350  Numbers that are the sum of 5 positive 5th powers.
 * https://oeis.org/A003350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- mark every sum x1^5+...+x5^5 <= L (1 <= x1 <= ... <=
 *            x5) in a sieve, list marked values in increasing order, double L
 *            until n terms exist
 * Range:     u8 n <= 7, u16 n <= 684, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a3350_terms = NULL;
static idx_t a3350_count = 0;
static uint64_t a3350_limit = 0;

static void a3350_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t pw[256];
    int K = 0;
    int i, j, k, l, m;
    uint64_t v, cnt;
    if (!mark) return;
    for (v = 1; v < 256; v++) {
        uint64_t p = v * v * v * v * v;
        if (p > L) break;
        pw[K++] = p;
    }
    for (i = 0; i < K; i++) {
        uint64_t s1 = pw[i];
        for (j = i; j < K && s1 + pw[j] <= L; j++) {
            uint64_t s2 = s1 + pw[j];
            for (k = j; k < K && s2 + pw[k] <= L; k++) {
                uint64_t s3 = s2 + pw[k];
                for (l = k; l < K && s3 + pw[l] <= L; l++) {
                    uint64_t s4 = s3 + pw[l];
                    for (m = l; m < K && s4 + pw[m] <= L; m++)
                        mark[s4 + pw[m]] = 1;
                }
            }
        }
    }
    cnt = 0;
    for (v = 1; v <= L; v++)
        if (mark[v]) cnt++;
    free(a3350_terms);
    a3350_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a3350_count = 0;
    for (v = 1; v <= L; v++)
        if (mark[v]) a3350_terms[a3350_count++] = v;
    a3350_limit = L;
    free(mark);
}

seq_t a(idx_t n) {
    if (n == 0) n = 1;
    while (a3350_count < n) {
        uint64_t L = a3350_limit ? a3350_limit * 2 : ((uint64_t)1 << 16);
        a3350_build(L);
    }
    return (seq_t)a3350_terms[n - 1];
}
