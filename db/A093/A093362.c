/*
 * A093362  Increasing gaps in A038593 (upper terms).
 * https://oeis.org/A093362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = upper term of the n-th record gap in the sorted list
 *            A038593 (differences of two positive cubes occurring at most 3
 *            ways); computed by sieve/sort of all cube differences up to a
 *            growing limit
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int A093362_cmp(const void *v1, const void *v2)
{
    const seq_t *x = (const seq_t *)v1;
    const seq_t *y = (const seq_t *)v2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static seq_t *A093362_rec = NULL;
static unsigned long long A093362_cnt = 0;
static bool A093362_init = false;
static unsigned long long A093362_L = 0;

static void A093362_build(unsigned long long L)
{
    unsigned long long total = 0, b, a;
    for (b = 1; ; ++b) {
        unsigned long long b3 = b * b * b;
        if ((b + 1) * (b + 1) * (b + 1) - b3 > L) break;
        for (a = b + 1; a * a * a - b3 <= L; ++a) total++;
    }
    seq_t *diffs = mem_new(seq_t, total + 1);
    unsigned long long idx = 0;
    for (b = 1; ; ++b) {
        unsigned long long b3 = b * b * b;
        if ((b + 1) * (b + 1) * (b + 1) - b3 > L) break;
        for (a = b + 1; a * a * a - b3 <= L; ++a) {
            diffs[idx++] = (seq_t)(a * a * a - b3);
        }
    }
    mem_qsort(diffs, (size_t)total, sizeof(seq_t), A093362_cmp);

    /* keep values with multiplicity <= 3, in place */
    unsigned long long k = 0, i = 0;
    while (i < total) {
        unsigned long long j = i + 1;
        while (j < total && diffs[j] == diffs[i]) ++j;
        if (j - i <= 3) diffs[k++] = diffs[i];
        i = j;
    }

    if (A093362_rec) mem_free(A093362_rec);
    A093362_rec = mem_new(seq_t, k + 1);
    A093362_cnt = 0;
    seq_t best = 0;
    for (i = 1; i < k; ++i) {
        seq_t gap = diffs[i] - diffs[i - 1];
        if (gap > best) {
            best = gap;
            A093362_rec[A093362_cnt++] = diffs[i];
        }
    }
    mem_free(diffs);
    A093362_L = L;
}

seq_t a(idx_t n)
{
    if (!A093362_init) {
        A093362_build(1000000ULL);
        A093362_init = true;
    }
    while ((unsigned long long)n >= A093362_cnt) {
        A093362_build(A093362_L * 4ULL);
    }
    return A093362_rec[n];
}
