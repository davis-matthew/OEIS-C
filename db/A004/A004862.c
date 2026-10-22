/*
 * A004862  Numbers that are the sum of at most 11 nonzero 6th powers.
 * https://oeis.org/A004862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a(n) is the n-th number v >= 0 whose minimal count of
 *            nonzero 6th powers summing to v is <= 11 (min-count knapsack DP
 *            over v)
 * Range:     u8 n <= 42, u16 n <= 4670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *A004862_terms = NULL;
static idx_t A004862_count = 0;

static void A004862_build(long long limit) {
    long long pw[64];
    int np = 0;
    for (long long b = 1; np < 64; b++) {
        long long p = b * b * b * b * b * b;
        if (p > limit) break;
        pw[np++] = p;
    }

    unsigned char *d = (unsigned char *)malloc((size_t)(limit + 1));
    idx_t cnt = 1;
    d[0] = 0;
    for (long long v = 1; v <= limit; v++) {
        unsigned char best = 12;
        for (int i = 0; i < np && pw[i] <= v; i++) {
            unsigned char c = (unsigned char)(d[v - pw[i]] + 1);
            if (c < best) best = c;
        }
        d[v] = best;
        if (best <= 11) cnt++;
    }

    free(A004862_terms);
    A004862_terms = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
    idx_t k = 0;
    for (long long v = 0; v <= limit; v++)
        if (d[v] <= 11) A004862_terms[k++] = (seq_t)v;
    A004862_count = cnt;
    free(d);
}

seq_t a(idx_t n) {
    static long long limit = 0;
    while (n > A004862_count) {
        limit = (limit == 0) ? 65536LL : limit * 4;
        A004862_build(limit);
    }
    return A004862_terms[n - 1];
}
