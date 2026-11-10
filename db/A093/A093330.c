/*
 * A093330  Maximal values in A038598.
 * https://oeis.org/A093330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th left-to-right maximum (record) of A038598, the first
 *            differences of the sorted distinct values x^3-y^3 (x>y>0),
 *            preceded by 0. Computed by sieve and sort with a doubling limit
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int cmp_u64_a093330(const void *p, const void *q)
{
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x < y) ? -1 : (x > y);
}

static uint64_t *rec_a093330 = NULL;
static size_t rec_cnt_a093330 = 0;

static void build_a093330(uint64_t N, size_t need)
{
    (void)need;
    size_t cnt = 0;
    for (uint64_t b = 1; 3 * b * b + 3 * b + 1 <= N; ++b) {
        uint64_t b3 = b * b * b;
        for (uint64_t a = b + 1; a * a * a - b3 <= N; ++a) cnt++;
    }
    uint64_t *v = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    if (!v) return;
    size_t pos = 0;
    v[pos++] = 0;
    for (uint64_t b = 1; 3 * b * b + 3 * b + 1 <= N; ++b) {
        uint64_t b3 = b * b * b;
        for (uint64_t a = b + 1; a * a * a - b3 <= N; ++a)
            v[pos++] = a * a * a - b3;
    }
    qsort(v, pos, sizeof(uint64_t), cmp_u64_a093330);
    size_t u = 0;
    for (size_t i = 0; i < pos; ++i)
        if (u == 0 || v[i] != v[u - 1]) v[u++] = v[i];
    free(rec_a093330);
    rec_a093330 = (uint64_t *)malloc((u + 1) * sizeof(uint64_t));
    rec_cnt_a093330 = 0;
    if (rec_a093330) {
        uint64_t best = 0;
        for (size_t i = 1; i < u; ++i) {
            uint64_t g = v[i] - v[i - 1];
            if (g > best) {
                best = g;
                rec_a093330[rec_cnt_a093330++] = g;
            }
        }
    }
    free(v);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > rec_cnt_a093330) {
        static uint64_t curN = 0;
        uint64_t N = curN ? curN * 4 : ((uint64_t)1 << 20);
        const uint64_t LIMIT = (uint64_t)1 << 36;
        while (true) {
            build_a093330(N, (size_t)n);
            curN = N;
            if (rec_cnt_a093330 >= n || N >= LIMIT) break;
            N *= 4;
        }
    }
    if (n > rec_cnt_a093330) return 0;
    return (seq_t)rec_a093330[(size_t)(n - 1)];
}
