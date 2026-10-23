/*
 * A105475  Triangle read by rows: T(n,k) is number of compositions of n into
 *          k parts when each even part can be of two kinds.
 * https://oeis.org/A105475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is the (row,col) entry of the triangle T(n,k) generated
 *            by * T(n,k) = coeff of x^n in (x(1+2x)/(1-x^2))^k. * The
 *            triangle is read row‑wise: index n is flattened as * n → row r
 *            where r(r+1)/2 ≤ n < (r+1)(r+2)/2 * col c = n - r(r+1)/2. * a(n)
 *            = T(r,c). T(n,k) counts compositions of n into k...
 * Range:     u8 n <= 49, u16 n <= 143, u32 n <= 478, u64 n <= 1679,
 *            u128 n <= 6268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

/* ensure we can compute up to the required row */
static seq_t **T = NULL;          /* triangular array: T[n][k] */
static idx_t rows_allocated = 0;  /* number of rows currently allocated (T[0..rows_allocated-1]) */

/* binary search for the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t row_of_idx(idx_t n)
{
    idx_t low = 0, high = 1;
    /* grow high until it overshoots */
    while (((unsigned __int128)high * (high + 1) / 2) <= n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* compute rows up to 'r_needed' (inclusive) */
static void ensure_rows(idx_t r_needed)
{
    if (r_needed < rows_allocated)
        return;

    /* resize the array of pointers to rows */
    T = (seq_t **)realloc(T, sizeof(seq_t *) * (r_needed + 1));

    for (idx_t n = rows_allocated; n <= r_needed; ++n) {
        /* allocate the row with n+1 entries (k=0..n) */
        T[n] = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        /* initialise to zero */
        for (idx_t k = 0; k <= n; ++k)
            T[n][k] = 0;

        if (n == 0) {
            T[0][0] = (seq_t)1;
            continue;
        }

        /* T[n][0] = 0 already set */

        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = 0;
            /* last part can be at most m_max so that remaining n-m can accommodate k-1 parts */
            idx_t m_max = n - (k - 1);
            for (idx_t m = 1; m <= m_max; ++m) {
                seq_t prev = T[n - m][k - 1];
                seq_t w = (m & 1) ? (seq_t)1 : (seq_t)2;  /* 1 for odd, 2 for even */
                sum += prev * w;
            }
            T[n][k] = sum;
        }
    }
    rows_allocated = r_needed + 1;
}

seq_t a(idx_t n)
{
    idx_t row = row_of_idx(n);
    idx_t col = n - row * (row + 1) / 2;
    ensure_rows(row);
    return T[row][col];
}
