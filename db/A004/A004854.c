/*
 * A004854  Numbers that are the sum of at most 3 nonzero 6th powers.
 * https://oeis.org/A004854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search: sort all sums x^6+y^6+z^6 with 0 <= x <= y <= z <=
 *            m (0 = term absent); every value below (m+1)^6 is then complete,
 *            and m is doubled until there are at least n such values
 * Range:     u8 n <= 10, u16 n <= 75, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *tab = NULL;
static idx_t tab_len = 0;

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static void build(idx_t n) {
    uint64_t m = 8;
    for (;;) {
        uint64_t *p = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
        for (uint64_t i = 0; i <= m; i++)
            p[i] = i * i * i * i * i * i;
        uint64_t lim = (m + 1) * (m + 1) * (m + 1);
        lim = lim * lim;
        size_t cap = (size_t)((m + 1) * (m + 2) * (m + 3) / 6);
        uint64_t *s = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t cnt = 0;
        for (uint64_t i = 0; i <= m; i++)
            for (uint64_t j = i; j <= m; j++) {
                uint64_t ij = p[i] + p[j];
                if (ij >= lim) break;
                for (uint64_t k = j; k <= m; k++) {
                    uint64_t v = ij + p[k];
                    if (v >= lim) break;
                    s[cnt++] = v;
                }
            }
        free(p);
        qsort(s, cnt, sizeof(uint64_t), cmp_u64);
        size_t u = 0;
        for (size_t i = 0; i < cnt; i++)
            if (u == 0 || s[i] != s[u - 1])
                s[u++] = s[i];
        if ((idx_t)u >= n) {
            if (tab) free(tab);
            tab = s;
            tab_len = (idx_t)u;
            return;
        }
        free(s);
        m *= 2;
    }
}

seq_t a(idx_t n) {
    if (tab == NULL || n > tab_len)
        build(n);
    return (seq_t)tab[n - 1];
}
