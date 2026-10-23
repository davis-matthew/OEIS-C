/*
 * A194838  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=sqrt(3).
 * https://oeis.org/A194838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at position n in the concatenated triangular
 *            array whose row r holds the permutation of (1..r) sorted by the
 *            fractional parts of k*sqrt(3) (k=1..r)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const long double SQRT3 = 1.732050807568877293527446341505872037164L;

typedef struct {
    idx_t k;
    long double frac;
} pair_t;

static int cmp_pair(const void *p1, const void *p2)
{
    const pair_t *a = (const pair_t *)p1;
    const pair_t *b = (const pair_t *)p2;
    if (a->frac < b->frac) return -1;
    if (a->frac > b->frac) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for n==0, though offset is 1 */

    /* Find row r such that T(r-1) < n <= T(r), where T(r)=r*(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Zero‑based position within the row */
    idx_t prev_total = ((r - 1) * r) / 2;
    idx_t pos = n - prev_total - 1;

    /* Build array of pairs (k, fractional part of k*sqrt(3)) for k=1..r */
    pair_t *arr = (pair_t *)malloc(r * sizeof(pair_t));
    if (!arr) return (seq_t)0;  /* memory failure – return 0 as fallback */

    idx_t k;
    for (k = 1; k <= r; ++k) {
        long double prod = (long double)k * SQRT3;
        idx_t intpart = (idx_t)prod;          /* floor(product) */
        arr[k - 1].k = k;
        arr[k - 1].frac = prod - (long double)intpart;
    }

    /* Sort the pairs by their fractional part */
    qsort(arr, (size_t)r, sizeof(pair_t), cmp_pair);

    /* Retrieve the answer */
    seq_t result = (seq_t)arr[pos].k;

    free(arr);
    return result;
}
