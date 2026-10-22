/*
 * A004833  Numbers that are the sum of at most 4 nonzero 4th powers.
 * https://oeis.org/A004833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every x <= L that is s + t, where s and t are each
 *            a sum of at most 2 nonzero 4th powers (0 allowed); the marked x
 *            in increasing order are the terms. L doubles until there are at
 *            least n terms
 * Range:     u8 n <= 33, u16 n <= 2735, u32 n <= 9676, u64 n <= 9676,
 *            u128 n <= 9676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A4833_terms = NULL;
static idx_t A4833_cnt = 0;

static void A4833_build(uint64_t L) {
    uint64_t *p4, *s2;
    unsigned char *mark;
    idx_t np = 0, ns = 0, i, j;
    uint64_t b, x;

    p4 = (uint64_t *)malloc(64 * sizeof(uint64_t));
    for (b = 1; b * b * b * b <= L; b++) p4[np++] = b * b * b * b;

    s2 = (uint64_t *)malloc((np * (np + 1) / 2 + np + 1) * sizeof(uint64_t));
    s2[ns++] = 0;
    for (i = 0; i < np; i++) {
        s2[ns++] = p4[i];
        for (j = i; j < np; j++) {
            if (p4[i] + p4[j] <= L) s2[ns++] = p4[i] + p4[j];
        }
    }

    mark = (unsigned char *)calloc((size_t)L + 1, 1);
    for (i = 0; i < ns; i++) {
        for (j = i; j < ns; j++) {
            uint64_t s = s2[i] + s2[j];
            if (s <= L) mark[s] = 1;
        }
    }

    free(A4833_terms);
    A4833_cnt = 0;
    for (x = 0; x <= L; x++) if (mark[x]) A4833_cnt++;
    A4833_terms = (uint64_t *)malloc((A4833_cnt + 1) * sizeof(uint64_t));
    A4833_cnt = 0;
    for (x = 0; x <= L; x++) if (mark[x]) A4833_terms[A4833_cnt++] = x;

    free(mark);
    free(s2);
    free(p4);
}

seq_t a(idx_t n) {
    static uint64_t lim = 0;
    if (lim == 0) {
        lim = 1u << 16;
        A4833_build(lim);
    }
    while (A4833_cnt < n) {
        lim *= 2;
        A4833_build(lim);
    }
    return (seq_t)A4833_terms[n - 1];
}
