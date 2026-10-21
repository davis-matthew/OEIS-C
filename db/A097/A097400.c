/*
 * A097400  Number of different values that can be assumed by the determinant
 *          of a 3 X 3 matrix whose elements are all permutations of the
 *          consecutive integers in the range (n-4,n+4).
 * https://oeis.org/A097400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   cmp64, determinant
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For n ≥ 167 return 5761 (the values stabilize);
   for 0 ≤ n < 167 enumerate all 362 880 permutations of the
   nine consecutive integers (n‑4)…(n+4) and count distinct 3×3
   determinant values. */

/* Standard headers required by the build environment. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp64.h"
#include "../utils/determinant.h"

/* Generate next lexicographic permutation of an array of 9 ints.
   Return 1 if a next permutation exists, 0 otherwise.                     */
static int next_perm(int *a)
{
    /* Find the largest index i such that a[i] < a[i+1]. */
    int i = 7;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return 0;

    /* Find the largest index j > i such that a[j] > a[i]. */
    int j = 8;
    while (a[j] <= a[i]) --j;

    /* Swap a[i] and a[j]. */
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    /* Reverse the suffix starting at i+1. */
    int l = i + 1, r = 8;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l;
        --r;
    }
    return 1;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Stabilized value for large n. */
    if (n >= 167)
        return (seq_t)5761u;

    /* Memoization tables for the 167 finite values. */
    static seq_t cache[167];
    static unsigned char computed[167] = {0};

    /* Return cached value if available. */
    if (computed[(idx_t)n])
        return cache[(idx_t)n];

    /* Build the list of consecutive integers. */
    int base = (int)n - 4;            /* n ≤ 166, fits in int safely. */
    int vals[9];
    for (int i = 0; i < 9; ++i)
        vals[i] = base + i;

    /* Enumerate all permutations and store their determinants. */
    const int PERMS = 362880;          /* 9! */
    int64_t *dets = (int64_t *)malloc((size_t)PERMS * sizeof(int64_t));
    if (!dets) /* fatal; caller cannot recover, but avoid crash. */
        return (seq_t)0u;

    int count = 0;
    dets[count++] = determinant(vals);

    while (next_perm(vals))
        dets[count++] = determinant(vals);

    /* Sort and count distinct values. */
    qsort(dets, (size_t)count, sizeof(int64_t), cmp64);

    seq_t uniq = 1;                   /* at least one determinant exists. */
    for (int i = 1; i < count; ++i)
        if (dets[i] != dets[i - 1])
            ++uniq;

    /* Store result and clean up. */
    free(dets);
    cache[(idx_t)n] = uniq;
    computed[(idx_t)n] = 1;

    return uniq;
}
