/*
 * A003352  Numbers that are the sum of 7 positive 5th powers.
 * https://oeis.org/A003352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. Numbers <= L that are a sum of exactly 7 positive 5th
 *            powers, found by 7 rounds of reachability DP; L doubles until n
 *            terms are found
 * Range:     u8 n <= 9, u16 n <= 2303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *a3352_terms = NULL;
static idx_t a3352_count = 0;
static uint64_t a3352_limit = 0;

static void a3352_build(uint64_t L) {
    unsigned char *prev = (unsigned char *)calloc((size_t)(L + 1), 1);
    unsigned char *cur = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t p5[64];
    int np = 0;
    uint64_t i, v;
    int k, j;
    idx_t cnt = 0;
    for (i = 1;; i++) {
        uint64_t q = i * i * i * i * i;
        if (q > L) break;
        p5[np++] = q;
    }
    prev[0] = 1;
    for (k = 1; k <= 7; k++) {
        memset(cur, 0, (size_t)(L + 1));
        for (v = 0; v <= L; v++) {
            if (!prev[v]) continue;
            for (j = 0; j < np; j++) {
                uint64_t w = v + p5[j];
                if (w > L) break;
                cur[w] = 1;
            }
        }
        { unsigned char *t = prev; prev = cur; cur = t; }
    }
    for (v = 0; v <= L; v++) if (prev[v]) cnt++;
    free(a3352_terms);
    a3352_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (v = 0; v <= L; v++) if (prev[v]) a3352_terms[cnt++] = v;
    a3352_count = cnt;
    a3352_limit = L;
    free(prev);
    free(cur);
}

seq_t a(idx_t n) {
    uint64_t L = a3352_limit ? a3352_limit : 65536;
    if (n < 1) return (seq_t)0;
    while (a3352_limit == 0 || a3352_count < n) {
        if (a3352_limit != 0) L = a3352_limit * 2;
        a3352_build(L);
    }
    return (seq_t)a3352_terms[n - 1];
}
