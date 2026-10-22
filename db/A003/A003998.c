/*
 * A003998  Numbers that are a sum of distinct positive cubes in more than one
 *          way.
 * https://oeis.org/A003998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve -- count representations of each m as a sum of distinct
 *            positive cubes (0/1 knapsack, counts capped at 2); a(n) = n-th m
 *            with count >= 2
 * Range:     u8 n <= 4, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *g_terms = NULL;
static idx_t g_count = 0;

static void build(long long limit) {
    unsigned char *cnt = (unsigned char *)calloc((size_t)(limit + 1), 1);
    long long i, s, total = 0;
    if (!cnt) return;
    cnt[0] = 1;
    for (i = 1; i * i * i <= limit; i++) {
        long long c = i * i * i;
        for (s = limit; s >= c; s--) {
            unsigned v = (unsigned)cnt[s] + (unsigned)cnt[s - c];
            cnt[s] = (unsigned char)(v > 2 ? 2 : v);
        }
    }
    for (s = 1; s <= limit; s++)
        if (cnt[s] >= 2) total++;
    free(g_terms);
    g_terms = (long long *)malloc((size_t)(total + 1) * sizeof(long long));
    g_count = 0;
    if (g_terms) {
        for (s = 1; s <= limit; s++)
            if (cnt[s] >= 2) g_terms[++g_count] = s;
    }
    free(cnt);
}

seq_t a(idx_t n) {
    long long limit = 1LL << 15;
    while (g_count < n) {
        while ((idx_t)limit < n * 2 + 100000) limit *= 2;
        build(limit);
        limit *= 2;
    }
    return (seq_t)g_terms[n];
}
