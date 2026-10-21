/*
 * A239355  Number of unit hypercubes, aligned with a four-dimensional
 *          Cartesian mesh, partially enclosed along the edge of the first
 *          2^4-ant of a hypersphere centered at the origin, ordered by
 *          increasing radius.
 * https://oeis.org/A239355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of unit hypercubes in the first *
 *            16‑octant of a 4‑dimensional unit mesh that are *partially*
 *            filled * by a hypersphere of radius r, where r depends on n: * *
 *            n odd : r² = (n–1)/2 (exact radius) * n even : r²∈((n–2)/2 ,
 *            (n–2)/2 + 1) (intermediate radius) * * The count is co...
 * Range:     u8 n <= 51, u16 n <= 201, u32 n <= 201, u64 n <= 201,
 *            u128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global tables for precomputation */
static uint64_t *counts = NULL;      /* flattened [S][P] */
static uint64_t *pref   = NULL;      /* prefix sums over P */
static uint64_t *total  = NULL;      /* total per S */
static int preparedK   = -1;         /* largest K that has been precomputed */
static int preparedCols = 0;         /* number of columns = maxP+1 */

/* Build tables up to given maxK (inclusive) */
static void build_counts(int maxK)
{
    int Imax = (int)floor(sqrt((double)maxK));
    int cols = 4 * Imax + 1;                 /* max coordinate sum + 1 */

    size_t rows = (size_t)maxK + 1;
    size_t totalSize = rows * (size_t)cols;
    /* free old memory */
    free(counts);
    free(pref);
    free(total);
    counts = (uint64_t*)calloc(totalSize, sizeof(uint64_t));
    pref   = (uint64_t*)calloc(totalSize, sizeof(uint64_t));
    total  = (uint64_t*)calloc(rows,        sizeof(uint64_t));

    /* Count tuples with S ≤ maxK */
    for (int i = 0; i <= Imax; ++i) {
        int i2 = i * i;
        for (int j = 0; j <= Imax; ++j) {
            int j2 = j * j;
            for (int k = 0; k <= Imax; ++k) {
                int k2 = k * k;
                for (int l = 0; l <= Imax; ++l) {
                    int sumS = i2 + j2 + k2 + l*l;
                    if (sumS > maxK) continue;
                    int sumP = i + j + k + l;
                    counts[(size_t)sumS * cols + sumP] += 1;
                }
            }
        }
    }

    /* Compute prefix sums over P and totals over S */
    for (int S = 0; S <= maxK; ++S) {
        uint64_t acc = 0;
        for (int p = 0; p < cols; ++p) {
            acc += counts[(size_t)S * cols + p];
            pref[(size_t)S * cols + p] = acc;
        }
        total[S] = acc;
    }

    preparedK   = maxK;
    preparedCols = cols;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    bool odd = (n & 1);
    /* needed maximum S index for precomputation */
    int maxKNeeded = (int)((n - 2) / 2);
    if (maxKNeeded > preparedK) {
        build_counts(maxKNeeded);
    }

    seq_t result = 0;

    if (odd) {                               /* exact radius */
        int t = (int)((n - 1) / 2);
        int upperS = t - 1;
        if (upperS < 0) upperS = 0;
        for (int S = 0; S <= upperS && S <= preparedK; ++S) {
            int needed = t - 3 - S;
            int LB = (needed <= 0) ? 0 : (needed + 1) / 2;
            if (LB >= preparedCols) continue;
            uint64_t cnt = total[S];
            if (LB > 0) cnt -= pref[(size_t)S * preparedCols + (LB - 1)];
            result += (seq_t)cnt;
        }
    } else {                                 /* intermediate radius */
        int K = (int)((n - 2) / 2);
        for (int S = 0; S <= K && S <= preparedK; ++S) {
            int needed = K - 3 - S;
            int LB = (needed <= 0) ? 0 : (needed + 1) / 2;
            if (LB >= preparedCols) continue;
            uint64_t cnt = total[S];
            if (LB > 0) cnt -= pref[(size_t)S * preparedCols + (LB - 1)];
            result += (seq_t)cnt;
        }
    }

    return result;
}
