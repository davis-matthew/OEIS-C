/*
 * A004843  Numbers that are the sum of at most 3 positive 5th powers.
 * https://oeis.org/A004843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search - enumerate all sums i^5+j^5+k^5 (0<=i<=j<=k) up to
 *            a bound, sort, dedupe; a(n) is the n-th smallest
 * Range:     u8 n <= 13, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9976 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int A004843_cmp(const void *pa, const void *pb) {
    uint64_t x = *(const uint64_t *)pa, y = *(const uint64_t *)pb;
    return (x < y) ? -1 : (x > y ? 1 : 0);
}

static uint64_t *A004843_tab = NULL;
static size_t A004843_len = 0;
static uint64_t A004843_bound = 0;

static void A004843_build(uint64_t X) {
    uint64_t m = 0;
    while ((m + 1) * (m + 1) * (m + 1) * (m + 1) * (m + 1) <= X) m++;
    uint64_t *p = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= m; i++) p[i] = i * i * i * i * i;

    size_t cnt = 0;
    for (uint64_t i = 0; i <= m; i++)
        for (uint64_t j = i; j <= m && p[i] + p[j] <= X; j++)
            for (uint64_t k = j; k <= m && p[i] + p[j] + p[k] <= X; k++)
                cnt++;

    uint64_t *s = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
    size_t c = 0;
    for (uint64_t i = 0; i <= m; i++)
        for (uint64_t j = i; j <= m && p[i] + p[j] <= X; j++)
            for (uint64_t k = j; k <= m && p[i] + p[j] + p[k] <= X; k++)
                s[c++] = p[i] + p[j] + p[k];
    free(p);

    qsort(s, c, sizeof(uint64_t), A004843_cmp);
    size_t w = 0;
    for (size_t r = 0; r < c; r++)
        if (r == 0 || s[r] != s[r - 1]) s[w++] = s[r];

    free(A004843_tab);
    A004843_tab = s;
    A004843_len = w;
    A004843_bound = X;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if ((uint64_t)n > (uint64_t)A004843_len) {
        uint64_t X = A004843_bound ? A004843_bound : 1024;
        for (;;) {
            if (X > A004843_bound) A004843_build(X);
            if ((uint64_t)A004843_len >= (uint64_t)n) break;
            X *= 4;
        }
    }
    return (seq_t)A004843_tab[n - 1];
}
