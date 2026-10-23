/*
 * A144441  Triangle T(n,k) read by rows: T(n, k) = (4*n-4*k+1)*T(n-1, k-1) +
 *          (4*k-3)*T(n-1, k) + 4*T(n-2, k-1) with T(n, 1) = T(n, n) = 1.
 * https://oeis.org/A144441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where r is the row index satisfying r(r+1)/2 >=
 *            n, k = n - (r-1)r/2. T is defined by T(1,1) = 1 T(n,1) = T(n,n)
 *            = 1 T(n,k) = (4n-4k+1)*T(n-1,k-1) + (4k-3)*T(n-1,k) +
 *            4*T(n-2,k-1) (2 <= k <= n-1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 49, u64 n <= 126,
 *            u128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration for types supplied by build */
extern seq_t a(idx_t n);

/* Internal storage for triangle rows */
static seq_t **tri = NULL;
static idx_t tri_max = 0;   /* maximum row index computed */

/* Compute row index r and column k from global index n */
static void idx_to_rowcol(idx_t n, idx_t *r_out, idx_t *k_out) {
    idx_t low = 1, high = 1;
    /* Find high such that high*(high+1)/2 >= n */
    while (((__int128)high * (high + 1) / 2) < n) {
        high <<= 1;
    }
    /* Binary search for minimal high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    *r_out = low;
    __int128 prev = (__int128)(low - 1) * low / 2;
    *k_out = n - prev;
}

/* Ensure that rows up to 'up_to' are computed. */
static void ensure_rows(idx_t up_to) {
    if (up_to <= tri_max)
        return;

    /* Grow storage for row pointers */
    size_t new_size = (size_t)(up_to + 1);
    tri = (seq_t **)realloc(tri, new_size * sizeof(seq_t *));
    if (!tri) {
        /* Allocation failure – abort */
        abort();
    }
    for (idx_t r = tri_max + 1; r <= up_to; ++r) {
        /* Allocate row (1..r), ignore index 0 */
        seq_t *row = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
        if (!row) abort();
        row[1] = (seq_t)1;
        if (r > 1)
            row[r] = (seq_t)1;
        if (r >= 3) {
            for (idx_t k = 2; k < r; ++k) {
                seq_t lhs = (seq_t)(4 * r - 4 * k + 1) * tri[r - 1][k - 1];
                lhs += (seq_t)(4 * k - 3) * tri[r - 1][k];
                lhs += (seq_t)4 * tri[r - 2][k - 1];
                row[k] = lhs;
            }
        }
        tri[r] = row;
    }
    tri_max = up_to;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    idx_t r, k;
    idx_to_rowcol(n, &r, &k);
    ensure_rows(r);
    return tri[r][k];
}
