/*
 * A004848  Numbers that are the sum of at most 8 positive 5th powers.
 * https://oeis.org/A004848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve - for every v up to a growing limit, find the fewest
 *            positive 5th powers that sum to v (unbounded knapsack); a(n) is
 *            the n-th v whose count is <= 8, with a(1) = 0
 * Range:     u8 n <= 52, u16 n <= 8289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned long long *terms = NULL;
static idx_t nterms = 0;
static unsigned long long built_limit = 0;

static void build(unsigned long long L) {
    unsigned char *m = (unsigned char *)malloc((size_t)(L + 1));
    unsigned long long v, b, p, cnt = 0;
    memset(m, 255, (size_t)(L + 1));
    m[0] = 0;
    for (b = 1;; b++) {
        p = b * b * b * b * b;
        if (p > L) break;
        for (v = p; v <= L; v++) {
            unsigned char c = m[v - p];
            if (c < 8 && (unsigned char)(c + 1) < m[v]) m[v] = (unsigned char)(c + 1);
        }
    }
    for (v = 0; v <= L; v++) if (m[v] <= 8) cnt++;
    free(terms);
    terms = (unsigned long long *)malloc((size_t)(cnt ? cnt : 1) * sizeof(unsigned long long));
    nterms = 0;
    for (v = 0; v <= L; v++) if (m[v] <= 8) terms[nterms++] = v;
    free(m);
    built_limit = L;
}

seq_t a(idx_t n) {
    unsigned long long L = built_limit ? built_limit : 65536ULL;
    if (n < 1) return (seq_t)0;
    if (!terms) build(L);
    while (nterms < n) {
        L *= 2;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
