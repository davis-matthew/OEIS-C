/*
 * A003398  Numbers that are the sum of 9 positive 9th powers.
 * https://oeis.org/A003398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 9 positive 9th powers (bases
 *            nondecreasing) up to a limit, sort, deduplicate; double the
 *            limit until at least n terms are found
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3398_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3398_cmp.h"

static uint64_t *a3398_buf = NULL;
static size_t a3398_cnt = 0, a3398_cap = 0;
static uint64_t a3398_p9[200];
static int a3398_nb = 0;

static void a3398_gen(int k, int minb, uint64_t sum, uint64_t L) {
    int b;
    if (k == 0) {
        if (a3398_cnt >= a3398_cap) {
            a3398_cap = a3398_cap ? a3398_cap * 2 : 4096;
            a3398_buf = (uint64_t *)realloc(a3398_buf, a3398_cap * sizeof(uint64_t));
        }
        a3398_buf[a3398_cnt++] = sum;
        return;
    }
    for (b = minb; b <= a3398_nb; b++) {
        /* remaining k terms are each >= b^9 */
        if (sum + (uint64_t)k * a3398_p9[b] > L) break;
        a3398_gen(k - 1, b, sum + a3398_p9[b], L);
    }
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t nterms = 0;
    static uint64_t limit = 0;

    while ((idx_t)nterms < n) {
        size_t i, u;
        uint64_t p;
        int b;
        limit = limit ? limit * 2 : ((uint64_t)1 << 20);
        a3398_nb = 0;
        for (b = 1; b < 199; b++) {
            p = 1;
            for (i = 0; i < 9; i++) p *= (uint64_t)b;
            if (p > limit) break;
            a3398_p9[b] = p;
            a3398_nb = b;
        }
        a3398_cnt = 0;
        a3398_gen(9, 1, 0, limit);
        if (a3398_cnt > 0)
            qsort(a3398_buf, a3398_cnt, sizeof(uint64_t), a3398_cmp);
        u = 0;
        for (i = 0; i < a3398_cnt; i++)
            if (u == 0 || a3398_buf[i] != a3398_buf[u - 1])
                a3398_buf[u++] = a3398_buf[i];
        free(terms);
        terms = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
        for (i = 0; i < u; i++) terms[i] = a3398_buf[i];
        nterms = u;
    }
    return (seq_t)terms[n - 1];
}
