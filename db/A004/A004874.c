/*
 * A004874  Numbers that are the sum of at most 12 positive 7th powers.
 * https://oeis.org/A004874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: a(n) is the n-th number v >= 0 whose minimal count of
 *            positive 7th powers summing to v is <= 12 (DP over v, doubling
 *            the limit as needed)
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *g_terms = NULL;
static idx_t g_count = 0;
static long long g_limit = 0;

static void build(long long L) {
    unsigned char *cnt = (unsigned char *)malloc((size_t)(L + 1));
    long long pw[64];
    int np = 0;
    long long b, v;
    int i;
    for (b = 1; ; b++) {
        long long p = b * b * b * b * b * b * b;
        if (p > L) break;
        pw[np++] = p;
    }
    cnt[0] = 0;
    for (v = 1; v <= L; v++) {
        unsigned char best = 255;
        for (i = 0; i < np && pw[i] <= v; i++) {
            unsigned char c = cnt[v - pw[i]];
            if (c != 255 && (unsigned char)(c + 1) < best) best = (unsigned char)(c + 1);
        }
        if (best > 13) best = 13; /* cap: anything > 12 is "not representable" */
        cnt[v] = best;
    }
    free(g_terms);
    g_count = 0;
    for (v = 0; v <= L; v++)
        if (cnt[v] <= 12) g_count++;
    g_terms = (long long *)malloc((size_t)(g_count + 1) * sizeof(long long));
    g_count = 0;
    for (v = 0; v <= L; v++)
        if (cnt[v] <= 12) g_terms[g_count++] = v;
    free(cnt);
    g_limit = L;
}

seq_t a(idx_t n) {
    if (g_terms == NULL) build(1 << 16);
    while (g_count < n) build(g_limit * 2);
    return (seq_t)(idx_t)g_terms[n - 1];
}
