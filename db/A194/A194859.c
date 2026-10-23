/*
 * A194859  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {e}, {2e}, ..., {ne}.
 * https://oeis.org/A194859
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

/* 
 * Computes A194859: a(n) = the term at position n in the flattened array of
 * permutations of (1,2,…,r) obtained by ordering the fractional parts of k*e,
 * where e is Euler's number.  The r is the row that contains n (r(r+1)/2 ≥ n,
 * (r−1)r/2 < n).  For that row, the permutation is sorted by increasing
 * fractional part {k*e} for k = 1..r, and the requested element is the
 * pos‑th (1‑based) entry of that sorted list, where
 * pos = n − (r−1)r/2.  A long double approximation of e is used for the
 * fractional parts; this is sufficient for all rows with r ≤ 141 (n ≤ 10000).
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static const long double e_val = 2.71828182845904523536028747135266249775724709369995L;

typedef struct { long double f; idx_t idx; } fp_t;

static int cmp_fp(const void *a, const void *b)
{
    const fp_t *pa = (const fp_t *)a;
    const fp_t *pb = (const fp_t *)b;
    if (pa->f < pb->f) return -1;
    if (pa->f > pb->f) return 1;
    return (pa->idx < pb->idx) ? -1 : (pa->idx > pb->idx) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrtl((long double)(8 * n + 1)) - 1) / 2);
    while (((r * (r + 1)) / 2) < n) ++r;

    idx_t s_prev = ((r - 1) * r) / 2;   /* sum up to row r-1 */
    idx_t pos = n - s_prev;             /* 1‑based position in row r */

    /* Build array of fractional parts {k*e} for k = 1..r */
    fp_t *arr = (fp_t *)malloc((size_t)r * sizeof(fp_t));
    if (!arr) return (seq_t)0;           /* out of memory -> return 0 as fallback */

    for (idx_t k = 1; k <= r; ++k) {
        long double prod = (long double)k * e_val;
        arr[k - 1].f = fmodl(prod, 1.0L);
        arr[k - 1].idx = k;
    }

    /* Sort by fractional part */
    qsort(arr, (size_t)r, sizeof(fp_t), cmp_fp);

    seq_t result = (seq_t)arr[pos - 1].idx;   /* 0‑based index into sorted array */

    free(arr);
    return result;
}
