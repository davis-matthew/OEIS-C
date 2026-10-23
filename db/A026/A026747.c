/*
 * A026747  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0; for n
 *          >= 2 and 1 <= k <= n-1, T(n,k) = T(n-1,k-1) + T(n-2,k-1) +
 *          T(n-1,k) if n is even and 1 <= k <= n/2, else T(n,k) = T(n-1,k-1)
 *          + T(n-1,k).
 * https://oeis.org/A026747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 142, u32 n <= 508, u64 n <= 1795,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026747
   Triangular array T, read by rows.
   T(n,0)=T(n,n)=1;
   For n ≥ 2, 1 ≤ k ≤ n−1:
     if n even and k ≤ n/2    →  T(n,k) = T(n−1,k−1) + T(n−2,k−1) + T(n−1,k)
     otherwise                →  T(n,k) = T(n−1,k−1) + T(n−1,k);
   a(n) is the element of T read by rows, i.e. if r is the unique
   integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2, then a(n)=T(r, n−r(r+1)/2). */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the triangle */
static seq_t **T = NULL;          /* T[n] points to row n (array of n+1 entries) */
static idx_t rows_allocated = 0;  /* number of row pointers allocated   */
static idx_t rows_computed = 0;   /* number of rows already initialized */

/* enlarge pointer array to hold rows 0 … r */
static void ensure_rows(idx_t r)
{
    if (r < rows_computed) return;               /* nothing missing */

    /* grow pointer array if necessary */
    if (r + 1 > rows_allocated) {
        size_t new_cap = (size_t)(r + 1);
        void *tmp = realloc(T, new_cap * sizeof(seq_t *));
        if (!tmp) abort();                       /* allocation failed */
        T = (seq_t **)tmp;
        rows_allocated = (idx_t)new_cap;
    }

    /* compute missing rows */
    for (idx_t n = rows_computed; n <= r; ++n) {
        T[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!T[n]) abort();

        if (n == 0) {
            T[n][0] = 1;
        } else if (n == 1) {
            T[n][0] = 1;
            T[n][1] = 1;
        } else {
            T[n][0] = 1;
            T[n][n] = 1;
            for (idx_t k = 1; k <= n - 1; ++k) {
                if ((n & 1) == 0 && k <= n / 2) {
                    T[n][k] = T[n - 1][k - 1] + T[n - 2][k - 1] + T[n - 1][k];
                } else {
                    T[n][k] = T[n - 1][k - 1] + T[n - 1][k];
                }
            }
        }
    }
    rows_computed = r + 1;
}

/* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t row_of(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* exponential search for upper bound */
    while (true) {
        unsigned __int128 s = (unsigned __int128)high * (high + 1) / 2;
        if (s > n) break;
        low = high;
        high <<= 1;           /* assume high stays in range of idx_t */
    }

    /* binary search in [low,high] */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* external function required by harness */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t c = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    ensure_rows(r);
    return T[r][c];
}
