/*
 * A004859  Numbers that are the sum of at most 8 nonzero 6th powers.
 * https://oeis.org/A004859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve of minimal number of nonzero 6th powers summing to x (DP
 *            over 6th powers); a(n) is the n-th x needing at most 8 (x = 0
 *            counts)
 * Range:     u8 n <= 30, u16 n <= 1589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static uint64_t nterms = 0;

static void build(uint64_t limit)
{
    unsigned char *cnt = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t x, k, p, c;
    for (x = 0; x <= limit; x++) cnt[x] = 255;
    cnt[0] = 0;
    for (k = 1;; k++) {
        p = k * k * k;
        p = p * p;
        if (p > limit) break;
        for (x = p; x <= limit; x++) {
            unsigned char v = cnt[x - p];
            if (v < 8 && (unsigned char)(v + 1) < cnt[x]) cnt[x] = (unsigned char)(v + 1);
        }
    }
    c = 0;
    for (x = 0; x <= limit; x++)
        if (cnt[x] <= 8) c++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(c ? c : 1) * sizeof(uint64_t));
    nterms = 0;
    for (x = 0; x <= limit; x++)
        if (cnt[x] <= 8) terms[nterms++] = x;
    free(cnt);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 1u << 16;
    if (n < 1) return (seq_t)0;
    while (nterms < n) {
        if (terms != NULL) limit *= 2;
        build(limit);
    }
    return (seq_t)terms[n - 1];
}
