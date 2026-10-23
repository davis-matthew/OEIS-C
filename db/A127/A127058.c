/*
 * A127058  Triangle, read by rows, defined by: T(n,k) = Sum_{j=0..n-k-1}
 *          T(j+k,k)*T(n-j,k+1) for n > k >= 0, with T(n,n) = n+1.
 * https://oeis.org/A127058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle A127058: T(n,k) = Σ_{j=0..n-k-1} T(j+k,k) * T(n-j,k+1) for n>k≥0,
 * with T(n,n)=n+1.  Sequence a(n) is the flattened triangle read by rows.
 * Indexing: a(0)=1.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- helper: integer square root for uint64_t ---------- */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 4294967295ULL;  /* sqrt(ULLONG_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ---------- static memoization of the triangle ---------- */
static seq_t **triangle = NULL;     /* rows of the triangle */
static idx_t   max_row   = 0;        /* highest row computed so far */

/* Allocate rows up to and including row r.  Old rows are preserved. */
static void compute_rows_up_to(idx_t r)
{
    if (r < max_row) return;

    triangle = (seq_t **)realloc(triangle, (r + 1) * sizeof(seq_t *));
    if (!triangle) return;   /* allocation failure – unlikely in tests */

    if (triangle[0] == NULL) {
        triangle[0] = (seq_t *)malloc(sizeof(seq_t));
        if (!triangle[0]) return;
        triangle[0][0] = 1;   /* T(0,0) = 0+1 = 1 */
    }

    for (idx_t n = max_row + 1; n <= r; ++n) {
        triangle[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!triangle[n]) return;

        triangle[n][n] = (seq_t)(n + 1);   /* diagonal */

        for (int k = (int)n - 1; k >= 0; --k) {
            seq_t sum = 0;
            idx_t limit = n - (idx_t)k - 1;   /* j runs 0 .. limit */
            for (idx_t j = 0; j <= limit; ++j) {
                sum += triangle[j + k][k] * triangle[n - j][k + 1];
            }
            triangle[n][k] = sum;
        }
    }
    max_row = r;
}

/* ---------- oracle function for the sequence ---------- */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t s = floor_isqrt_u64(8 * n + 1);
    idx_t r = (s - 1) / 2;

    if (triangle == NULL) {
        triangle = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
        if (!triangle) return (seq_t)0;   /* allocation failure */
    }
    compute_rows_up_to(r);

    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t col   = n - start;        /* column within the row */
    return triangle[r][col];
}
