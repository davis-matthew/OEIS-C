/*
 * A144438  Triangle T(n,k) by rows: T(n, k) = (n-k+1)*T(n-1, k-1) + k*T(n-1,
 *          k) + T(n-2, k-1) with T(n, 1) = T(n, n) = 1.
 * https://oeis.org/A144438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (n-k+1)*T(n-1,k-1) + k*T(n-1,k) + T(n-2,k-1);
 *            T(n,1)=T(n,n)=1. The sequence a(n) is the nth element in
 *            row‑major order of this triangle
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 96, u64 n <= 220,
 *            u128 n <= 609, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t **tri_rows = NULL;
static size_t tri_cap  = 0;
static idx_t   max_row  = 0;

/* integer square root of an unsigned __int128, returning floor(sqrt(x)) */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;   /* upper bound */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid == 0 || mid <= x / mid)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo - 1);
}

/* map a 1‑based index to (row,k) of the triangle */
static void index_to_rowk(idx_t idx, idx_t *row, idx_t *k)
{
    /* solve n(n+1)/2 >= idx, i.e. n^2 + n - 2*idx >= 0 */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * idx;
    idx_t s = isqrt_u128(val);          /* floor(sqrt(1+8*idx)) */
    idx_t n = (s - 1) / 2;
    unsigned __int128 tri = (unsigned __int128)n * (n + 1) / 2;
    if (tri < idx) ++n;
    *row = n;
    *k   = idx - (unsigned __int128)(n - 1) * n / 2;
}

/* ensure the triangular array can hold up to 'need' rows */
static void ensure_rows_cap(idx_t need)
{
    if ((size_t)(need + 1) > tri_cap) {
        size_t new_cap = tri_cap ? tri_cap * 2 : 8;
        while (new_cap <= (size_t)(need + 1)) new_cap *= 2;
        tri_rows = (seq_t **)realloc(tri_rows, (new_cap + 1) * sizeof(seq_t *));
        for (size_t i = tri_cap; i <= new_cap; ++i) tri_rows[i] = NULL;
        tri_cap = new_cap;
    }
}

/* compute row 'n' and store it in tri_rows[n] */
static void compute_row(idx_t n)
{
    ensure_rows_cap(n);
    tri_rows[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *row = tri_rows[n];

    row[1] = 1;
    if (n > 1) row[n] = 1;
    if (n <= 2) return;

    for (idx_t k = 2; k <= n - 1; ++k) {
        seq_t term1 = (seq_t)(n - k + 1) * tri_rows[n - 1][k - 1];
        seq_t term2 = (seq_t)k * tri_rows[n - 1][k];
        seq_t term3 = tri_rows[n - 2][k - 1];
        row[k] = term1 + term2 + term3;
    }
}

seq_t a(idx_t index)
{
    if (index == 0) return (seq_t)0;  /* undefined, but safe */

    idx_t row, k;
    index_to_rowk(index, &row, &k);

    /* build rows up to 'row' if needed */
    for (idx_t i = max_row + 1; i <= row; ++i) compute_row(i);
    if (row > max_row) max_row = row;

    return tri_rows[row][k];
}
