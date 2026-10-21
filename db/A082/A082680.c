/*
 * A082680  Triangle read by rows: T(n,k) is the number of 2-stack sortable
 *          n-permutations with k runs.
 * https://oeis.org/A082680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = (r+k-1)!·(2r–k)! /
 *            (k!·(r+1–k)!·(2k–1)!·(2r–2k+1)!) The triangle is read
 *            row‑by‑row. For a given index n ≥ 1, find the row r and column k
 *            by: r = smallest integer with r(r+1)/2 ≥ n k = n – r(r–1)/2
 *            (1‑based) The value a(n) is the binomial ratio above, computed
 *            by canc...
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 142, u64 n <= 419,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* locate row r and column k */
    idx_t r = 1;
    idx_t cum = 1;                  /* sum of rows 1..r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t before = ((r - 1) * r) / 2;    /* elements before row r */
    idx_t k  = n - before;              /* 1‑based column */

    /* build numerator factors: (r+k-1)! and (2r-k)! */
    size_t num_len = (size_t)(r + k - 1) + (size_t)(2 * r - k);
    uint64_t *num = (uint64_t *)malloc(num_len * sizeof(uint64_t));
    if (!num) return 0; /* fallback on OOM */
    size_t nc = 0;
    for (idx_t i = 1; i <= r + k - 1; ++i) num[nc++] = (uint64_t)i;
    for (idx_t i = 1; i <= 2 * r - k;   ++i) num[nc++] = (uint64_t)i;

    /* build denominator factors: k!, (r+1-k)!, (2k-1)!, (2r-2k+1)! */
    size_t den_len = (size_t)k + (size_t)(r + 1 - k) +
                     (size_t)(2 * k - 1) + (size_t)(2 * r - 2 * k + 1);
    uint64_t *den = (uint64_t *)malloc(den_len * sizeof(uint64_t));
    if (!den) { free(num); return 0; }
    size_t dc = 0;
    for (idx_t i = 1; i <= k;              ++i) den[dc++] = (uint64_t)i;
    for (idx_t i = 1; i <= r + 1 - k;      ++i) den[dc++] = (uint64_t)i;
    for (idx_t i = 1; i <= 2 * k - 1;      ++i) den[dc++] = (uint64_t)i;
    for (idx_t i = 1; i <= 2 * r - 2 * k + 1; ++i) den[dc++] = (uint64_t)i;

    /* cancel common factors between numerator and denominator */
    for (size_t i = 0; i < dc; ++i) {
        seq_t d = (seq_t)den[i];
        if (d <= 1) continue;
        for (size_t j = 0; j < nc && d > 1; ++j) {
            seq_t g = gcd((seq_t)num[j], d);
            if (g > 1) {
                num[j] = (uint64_t)((seq_t)num[j] / g);
                d     = d / g;
            }
        }
        /* All factors of this denominator term must have been cancelled */
        assert(d == 1);
    }

    /* multiply remaining numerator factors */
    seq_t result = 1;
    for (size_t i = 0; i < nc; ++i) {
        result *= (seq_t)num[i];
    }

    free(num);
    free(den);
    return result;
}
