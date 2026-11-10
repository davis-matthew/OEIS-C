/*
 * A093342  Increasing gaps in A038593 (lower terms).
 * https://oeis.org/A093342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record gap's lower term in A038593 (differences of
 *            positive cubes occurring in 1,2 or 3 ways); computed by
 *            enumerating all cube differences up to a limit, sorting,
 *            filtering by multiplicity <= 3, and tracking record gaps (limit
 *            grows until n records found)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int A093342_cmp(const void *p, const void *q)
{
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static uint64_t A093342_rec[512];
static size_t A093342_nrec = 0;
static uint64_t A093342_limit = 0;

static void A093342_build(uint64_t X)
{
    size_t cap = 1 << 16, cnt = 0;
    uint64_t *d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t a = 2; ; ++a) {
        uint64_t a3 = a * a * a;
        uint64_t b = a - 1;
        if (a3 - b * b * b > X) break;
        for (; b >= 1; --b) {
            uint64_t v = a3 - b * b * b;
            if (v > X) break;
            if (cnt == cap) {
                cap *= 2;
                d = (uint64_t *)realloc(d, cap * sizeof(uint64_t));
            }
            d[cnt++] = v;
        }
    }
    qsort(d, cnt, sizeof(uint64_t), A093342_cmp);
    size_t nrec = 0;
    bool have = false;
    uint64_t prev = 0, maxgap = 0;
    for (size_t i = 0; i < cnt; ) {
        size_t j = i + 1;
        while (j < cnt && d[j] == d[i]) ++j;
        if (j - i <= 3) {
            uint64_t cur = d[i];
            if (have) {
                uint64_t g = cur - prev;
                if (g > maxgap) {
                    maxgap = g;
                    if (nrec < 512) A093342_rec[nrec++] = prev;
                }
            }
            prev = cur;
            have = true;
        }
        i = j;
    }
    free(d);
    A093342_nrec = nrec;
    A093342_limit = X;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A093342_limit == 0) A093342_build((uint64_t)1 << 20);
    while (A093342_nrec < n && A093342_nrec < 512)
        A093342_build(A093342_limit * 4);
    return (seq_t)A093342_rec[n - 1];
}
