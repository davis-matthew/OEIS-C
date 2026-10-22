/*
 * A004837  Numbers that are the sum of at most 8 nonzero 4th powers.
 * https://oeis.org/A004837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a(n) is the n-th x >= 0 whose minimal number of nonzero
 *            4th powers summing to x is <= 8 (DP over x with cnt[0]=0,
 *            cnt[x]=1+min cnt[x-b^4])
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A004837_terms = NULL;
static uint64_t A004837_nterms = 0;

static void A004837_build(uint64_t limit) {
    uint8_t *cnt = (uint8_t *)malloc((size_t)(limit + 1));
    uint64_t pw[256];
    uint64_t np = 0, b, x, i, k;
    for (b = 1; b < 256; b++) {
        uint64_t p = b * b * b * b;
        if (p > limit) break;
        pw[np++] = p;
    }
    cnt[0] = 0;
    for (x = 1; x <= limit; x++) {
        uint8_t best = 9;
        for (i = 0; i < np && pw[i] <= x; i++) {
            uint8_t c = (uint8_t)(cnt[x - pw[i]] + 1);
            if (c < best) best = c;
        }
        cnt[x] = best;
    }
    k = 0;
    for (x = 0; x <= limit; x++)
        if (cnt[x] <= 8) k++;
    free(A004837_terms);
    A004837_terms = (uint64_t *)malloc((size_t)(k ? k : 1) * sizeof(uint64_t));
    k = 0;
    for (x = 0; x <= limit; x++)
        if (cnt[x] <= 8) A004837_terms[k++] = x;
    A004837_nterms = k;
    free(cnt);
}

seq_t a(idx_t n) {
    uint64_t limit = 1024;
    while (A004837_nterms < n) {
        A004837_build(limit);
        limit *= 2;
    }
    return (seq_t)A004837_terms[n - 1];
}
