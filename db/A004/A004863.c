/*
 * A004863  Numbers that are the sum of at most 12 nonzero 6th powers.
 * https://oeis.org/A004863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- sieve the least number of nonzero 6th powers c(x)
 *            needed to make x, by DP over x; a(n) = n-th x >= 0 with c(x) <=
 *            12
 * Range:     u8 n <= 46, u16 n <= 6267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;

static void build(uint64_t limit) {
    unsigned char *cnt = (unsigned char *)malloc((size_t)limit);
    uint64_t pw[64];
    int k = 0;
    uint64_t b, x;
    idx_t m = 0;
    if (cnt == NULL) return;
    for (b = 1; ; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (p >= limit) break;
        pw[k++] = p;
    }
    cnt[0] = 0;
    for (x = 1; x < limit; x++) {
        unsigned char best = 13;
        int j;
        for (j = 0; j < k && pw[j] <= x; j++) {
            unsigned char c = (unsigned char)(cnt[x - pw[j]] + 1);
            if (c < best) best = c;
        }
        cnt[x] = best;
    }
    for (x = 0; x < limit; x++)
        if (cnt[x] <= 12) m++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(m ? m : 1) * sizeof(uint64_t));
    nterms = 0;
    if (terms == NULL) { free(cnt); return; }
    for (x = 0; x < limit; x++)
        if (cnt[x] <= 12) terms[nterms++] = x;
    free(cnt);
}

seq_t a(idx_t n) {
    uint64_t limit = 1u << 16;
    if (n == 0) return 0;
    while (nterms < n) {
        build(limit);
        if (terms == NULL) return 0;
        limit *= 2;
    }
    return (seq_t)(idx_t)terms[n - 1];
}
