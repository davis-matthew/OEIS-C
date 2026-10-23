/*
 * A194911  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=2^(1/3).
 * https://oeis.org/A194911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A194911:  a(n) is the nth entry of the triangular array formed by
   permuting (1,2,…,r) in each row r according to increasing fractional
   parts of k*2^(1/3).  The function finds the row containing n, sorts
   1..r by the fractional parts and returns the element at the
   appropriate position. */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static const long double R_VAL = cbrtl(2.0L);   /* 2^(1/3) */

/* Comparator for qsort: order integers by the fractional part of i*R_VAL. */
static int cmp_frac(const void *a, const void *b)
{
    uint32_t ia = *(const uint32_t*)a;
    uint32_t ib = *(const uint32_t*)b;

    long double va = (long double)ia * R_VAL;
    long double vb = (long double)ib * R_VAL;
    va -= floorl(va);
    vb -= floorl(vb);

    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;   /* all fractions are distinct for irrational r */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; safety for non‑positive calls */

    /* Find the row r such that r*(r‑1)/2 + 1 <= n <= r*(r+1)/2 */
    /* Binary search on r (1 … n). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;          /* triangular number */
        if (tri >= n) high = mid;
        else          low = mid + 1;
    }
    idx_t r = low;  /* the required row */

    /* Position inside the row (0‑based). */
    idx_t offset = r * (r - 1) / 2 + 1;   /* first index of row r */
    idx_t pos0 = n - offset;              /* 0 <= pos0 < r */

    /* Allocate permutation array 1..r. */
    uint32_t *perm = (uint32_t*)malloc(r * sizeof(uint32_t));
    if (!perm) return 0;   /* allocation failed */
    for (uint32_t i = 0; i < r; ++i) perm[i] = i + 1;

    /* Sort by fractional parts. */
    qsort(perm, r, sizeof(uint32_t), cmp_frac);

    seq_t result = (seq_t)perm[pos0];

    free(perm);
    return result;
}
