/*
 * A055356  Triangle of increasing mobiles (circular rooted trees) with n
 *          nodes and k leaves.
 * https://oeis.org/A055356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n
 *            and k = n – r(r−1)/2. The triangle T(r,k) is defined by T(1,1)=1
 *            and T(r,k) = (r-2)*T(r-1,k-1) + k*T(r-1,k) (1≤k≤r, r≥2),
 *            T(r,k)=0 for k>r. The sequence a(n) is the triangle read row by
 *            row
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 85, u64 n <= 201,
 *            u128 n <= 515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *triangle = NULL;   /* flattened triangle */
static size_t  max_row  = 0;      /* highest row computed so far */

/* return row r and column k (1‑based) that correspond to index n */
static void
row_col(idx_t n, idx_t *r, idx_t *k)
{
    idx_t low = 1, high = 1;
    while ((idx_t)high * (high + 1) / 2 < n) ++high;  /* find high */
    low = 1;
    while (low < high) {                 /* binary search for r */
        idx_t mid = (low + high) / 2;
        if ((idx_t)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *r = low;
    idx_t prev_sum = (idx_t)(low - 1) * low / 2;
    *k = n - prev_sum;
}

/* ensure the triangle has been built up to row r */
static void
ensure_rows(idx_t r)
{
    if (r <= max_row) return;

    size_t new_total = (size_t)r * (r + 1) / 2;
    seq_t *new_tri = (seq_t *)realloc(triangle, new_total * sizeof(seq_t));
    if (!new_tri) { /* allocation failed – abort */
        exit(1);
    }
    triangle = new_tri;

    for (idx_t i = max_row + 1; i <= r; ++i) {
        size_t start_i = (size_t)(i - 1) * i / 2;      /* index of first element in row i */
        for (idx_t k = 1; k <= i; ++k) {
            if (i == 1) {
                triangle[start_i] = 1;
            } else if (k == 1) {
                triangle[start_i] = 1;
            } else if (k == i) {
                triangle[start_i + k - 1] = 0;
            } else {
                size_t prev_start = (size_t)(i - 2) * (i - 1) / 2;
                seq_t t1 = triangle[prev_start + (k - 2)];  /* T(i-1,k-1) */
                seq_t t2 = triangle[prev_start + (k - 1)];  /* T(i-1,k)   */
                seq_t term1 = (seq_t)(i - 2) * t1;
                seq_t term2 = (seq_t)k * t2;
                triangle[start_i + k - 1] = term1 + term2;
            }
        }
    }
    max_row = r;
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for 0 or negative indices */
    idx_t r, k;
    row_col(n, &r, &k);
    ensure_rows(r);
    size_t idx = (size_t)(r - 1) * r / 2 + (k - 1);
    return triangle[idx];
}
