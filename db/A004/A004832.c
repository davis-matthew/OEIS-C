/*
 * A004832  Numbers that are the sum of at most 3 nonzero 4th powers.
 * https://oeis.org/A004832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every s = i^4 + j^4 + k^4 <= L with 0 <= i <= j <=
 *            k, collect marked s in increasing order, and double L until n
 *            terms are found
 * Range:     u8 n <= 20, u16 n <= 700, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a004832_terms = NULL;
static idx_t a004832_count = 0;

static void a004832_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t p[2000];
    idx_t m = 0, i, j, k;
    uint64_t s, cnt = 0;
    while (1) {
        uint64_t b = (uint64_t)m;
        uint64_t b4 = b * b * b * b;
        if (b4 > L) break;
        p[m++] = b4;
    }
    for (i = 0; i < m; i++) {
        for (j = i; j < m; j++) {
            uint64_t ij = p[i] + p[j];
            if (ij > L) break;
            for (k = j; k < m; k++) {
                uint64_t t = ij + p[k];
                if (t > L) break;
                mark[t] = 1;
            }
        }
    }
    for (s = 0; s <= L; s++)
        if (mark[s]) cnt++;
    free(a004832_terms);
    a004832_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    a004832_count = 0;
    for (s = 0; s <= L; s++)
        if (mark[s]) a004832_terms[a004832_count++] = s;
    free(mark);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) {
        L = 4096;
        a004832_build(L);
    }
    while (a004832_count < n) {
        L *= 2;
        a004832_build(L);
    }
    return (seq_t)a004832_terms[n - 1];
}
