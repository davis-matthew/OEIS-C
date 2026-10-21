/*
 * A211402  Triangle T(n,k), 0<=k<=n, given by (0,2,0,4,0,6,0,8,0,10,0,...)
 *          DELTA (1, 2, 3, 4, 5, 6, 7, 8, 9,...) where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A211402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m, k) where m is the largest integer with m(m+1)/2 ≤ n
 *            and k = n - m(m+1)/2. T is defined by T(0,0) = 1 T(n,0) = 0
 *            (n>0) T(n,k) = 2*k*T(n-1,k) + (2*k-1)*T(n-1,k-1) (1 ≤ k ≤ n)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- Static storage for the triangle -----------------------------------*/
static seq_t **triangle_rows = NULL;   /* array of pointers to each row   */
static size_t   triangle_cap  = 0;      /* capacity of triangle_rows       */
static idx_t    triangle_count = 0;     /* number of rows already stored   */

/*--- Find the row index m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2 ----------*/
static idx_t
row_of_idx(idx_t n)
{
    idx_t low = 0, high = 1;

    /* expand high until it exceeds n */
    while ( ( (__int128)high * (high + 1) / 2 ) <= n ) {
        if (high > ((idx_t)-1) / 2) break;   /* prevent overflow of shift */
        high <<= 1;
    }

    /* binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) / 2 ) <= n )
            low = mid;
        else
            high = mid;
    }
    return low;   /* satisfies low*(low+1)/2 ≤ n < (low+1)*(low+2)/2 */
}

/*--- Ensure rows 0 … needed are computed and stored -------------------*/
static void
ensure_rows_upto(idx_t needed)
{
    if (triangle_count > needed) return;   /* already available */

    /* grow the array of row pointers if necessary */
    if (triangle_cap < (size_t)(needed + 1)) {
        size_t new_cap = triangle_cap ? triangle_cap : 1;
        while (new_cap <= (size_t)needed) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(triangle_rows, new_cap * sizeof(seq_t*));
        if (!tmp && new_cap > 0) exit(1);
        triangle_rows = tmp;
        triangle_cap  = new_cap;
    }

    /* compute missing rows up to 'needed' */
    for (idx_t r = triangle_count; r <= needed; ++r) {
        idx_t len = r + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) exit(1);

        if (r == 0) {          /* base row */
            row[0] = 1;        /* T(0,0) = 1 */
        } else {
            row[0] = 0;        /* T(n,0) = 0 for n>0 */
            seq_t *prev = triangle_rows[r - 1];   /* length = r */
            for (idx_t k = 1; k <= r; ++k) {
                seq_t prev_k  = (k < r) ? prev[k] : 0;      /* T(r-1,k) */
                seq_t prev_k1 = prev[k - 1];                /* T(r-1,k-1) */
                seq_t part1 = ((seq_t)2 * (seq_t)k) * prev_k;
                seq_t part2 = (((seq_t)2 * (seq_t)k) - (seq_t)1) * prev_k1;
                row[k] = part1 + part2;
            }
        }

        triangle_rows[r] = row;
        ++triangle_count;
    }
}

/*--- Requested OEIS sequence function --------------------------------*/
seq_t
a(idx_t n)
{
    /* locate the row and column corresponding to global index n */
    idx_t row    = row_of_idx(n);
    idx_t offset = (idx_t)( ((__int128)row * (row + 1)) / 2 );
    idx_t col    = n - offset;     /* 0 ≤ col ≤ row */

    /* make sure the needed row is available */
    ensure_rows_upto(row);

    /* return the requested entry */
    return triangle_rows[row][col];
}
