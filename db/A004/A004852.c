/*
 * A004852  Numbers that are the sum of at most 12 positive 5th powers.
 * https://oeis.org/A004852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: c(v) = min number of positive 5th powers summing to v
 *            (c(0) = 0, c(v) = 1 + min_{k>=1, k^5<=v} c(v - k^5)); a(n) =
 *            n-th v >= 0 with c(v) <= 12
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static size_t nterms = 0;
static uint64_t limit = 0;

static void build(uint64_t L) {
    unsigned char *c = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t pw[64];
    int np = 0;
    uint64_t k, v;
    size_t cnt = 0;
    int i;
    for (k = 1; k * k * k * k * k <= L && np < 64; k++)
        pw[np++] = k * k * k * k * k;
    c[0] = 0;
    for (v = 1; v <= L; v++) {
        unsigned best = 255;
        for (i = 0; i < np && pw[i] <= v; i++) {
            unsigned t = (unsigned)c[v - pw[i]] + 1;
            if (t < best) best = t;
        }
        c[v] = (unsigned char)(best > 200 ? 200 : best);
    }
    for (v = 0; v <= L; v++)
        if (c[v] <= 12) cnt++;
    free(terms);
    terms = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    nterms = 0;
    for (v = 0; v <= L; v++)
        if (c[v] <= 12) terms[nterms++] = v;
    free(c);
    limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (limit == 0) build(4096);
    while ((idx_t)nterms < n) build(limit * 2);
    return (seq_t)terms[n - 1];
}
