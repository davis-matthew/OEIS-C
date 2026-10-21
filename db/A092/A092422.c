/*
 * A092422  Triangle, read by rows, where T(n,k) equals the k-th term of the
 *          convolution of the (n-k)-th row with the (2k)-th Fibonacci
 *          polynomial (A011973).
 * https://oeis.org/A092422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binom, sqrt_uint128_to_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = Σ_{j=0}^{min(k,n-k)} C(k+j,k-j)*T(n-k,j)
 * with T(n,0)=1.  The sequence a(n) is the triangle read by rows
 * (row 0, then row 1, etc.). */

/* ----------------------------------------------------------------------
 * Required headers
 * ---------------------------------------------------------------------- */
#include <stdlib.h>
#include "../utils/binom.h"   /* Provides seq_t binom(idx_t a, idx_t b) */
#include "../utils/sqrt_uint128_to_idx_t.h"

/* ----------------------------------------------------------------------
 * Helper: compute row number r from linear index n.
 * Finds the unique r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
 * ---------------------------------------------------------------------- */
static inline idx_t row_from_index(idx_t n)
{
    __uint128_t val = (__uint128_t)8 * n + 1;
    idx_t s = sqrt_uint128_to_idx_t(val);   /* floor(sqrt(8*n+1)) */
    return (s - 1) / 2;
}

/* ----------------------------------------------------------------------
 * Cached storage for the triangle.
 * ---------------------------------------------------------------------- */
static seq_t *tri          = NULL;  /* flattened triangle elements */
static idx_t  tri_size      = 0;    /* allocated capacity */
static idx_t  max_row       = (idx_t)-1;  /* last fully computed row
                                            (sentinel = UINT64_MAX) */

/* ----------------------------------------------------------------------
 * Ensure that rows 0..need are computed and stored.
 * ---------------------------------------------------------------------- */
static void ensure_rows(idx_t need)
{
    if (max_row != (idx_t)-1 && need <= max_row)
        return;   /* already done */

    /* Grow storage if required */
    idx_t new_size = (need + 1) * (need + 2) / 2;
    if (!tri) {
        tri = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tri) exit(1);  /* allocation failure */
    } else if (new_size > tri_size) {
        tri = (seq_t *)realloc(tri, new_size * sizeof(seq_t));
        if (!tri) exit(1);
    }
    tri_size = new_size;

    /* Starting row for new computations */
    idx_t start = (max_row == (idx_t)-1) ? 0 : max_row + 1;

    for (idx_t i = start; i <= need; ++i) {
        idx_t base = i * (i + 1) / 2;        /* start index of row i */
        tri[base] = (seq_t)1;                /* T(i,0) = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            idx_t limit = (k < (i - k)) ? k : (i - k);
            seq_t sum = 0;
            idx_t prev_base = (i - k) * (i - k + 1) / 2;

            for (idx_t j = 0; j <= limit; ++j) {
                sum += binom(k + j, k - j) * tri[prev_base + j];
            }
            tri[base + k] = sum;
        }
    }
    max_row = need;
}

/* ----------------------------------------------------------------------
 * Public function: a(idx_t n)
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r      = row_from_index(n);
    idx_t offset = n - r * (r + 1) / 2;
    ensure_rows(r);
    return tri[r * (r + 1) / 2 + offset];
}
