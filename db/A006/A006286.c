/*
 * A006286  Numbers not of form p + 2^x + 2^y.
 * https://oeis.org/A006286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: mark every p + 2^x + 2^y (p prime, x,y >= 0) up to a
 *            growing limit; a(n) is the n-th unmarked positive integer
 * Range:     u8 n <= 6, u16 n <= 2195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a006286_terms = NULL;
static size_t a006286_count = 0;
static uint64_t a006286_limit = 0;

static void a006286_build(uint64_t L) {
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *rep = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t sums[4096];
    size_t ns = 0;
    uint64_t i, j, p, k;
    size_t t, cnt;

    for (i = 1; i <= L; i <<= 1)
        for (j = i; i + j <= L; j <<= 1)
            sums[ns++] = i + j;

    comp[0] = 1;
    if (L >= 1) comp[1] = 1;
    for (i = 2; i * i <= L; i++)
        if (!comp[i])
            for (j = i * i; j <= L; j += i) comp[j] = 1;

    for (p = 2; p <= L; p++) {
        if (comp[p]) continue;
        for (t = 0; t < ns; t++) {
            uint64_t v = p + sums[t];
            if (v <= L) rep[v] = 1;
        }
    }

    cnt = 0;
    for (k = 1; k <= L; k++)
        if (!rep[k]) cnt++;

    free(a006286_terms);
    a006286_terms = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (k = 1; k <= L; k++)
        if (!rep[k]) a006286_terms[cnt++] = k;

    a006286_count = cnt;
    a006286_limit = L;
    free(comp);
    free(rep);
}

seq_t a(idx_t n) {
    uint64_t L = a006286_limit ? a006286_limit : 4096;
    if (n < 1) return (seq_t)0;
    while (a006286_count < (size_t)n) {
        if (a006286_limit) L = a006286_limit * 2;
        a006286_build(L);
    }
    return (seq_t)a006286_terms[n - 1];
}
