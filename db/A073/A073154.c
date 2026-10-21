/*
 * A073154  Triangle of numbers relating two sequences (A073157 and A073155).
 * https://oeis.org/A073154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) runs through the entries of the triangle A073154 * (OEIS
 *            A073154) by row. For any n≥0, find the row r such that *
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, then k = n - r*(r+1)/2. * The
 *            triangle entry a(r,k) is defined by * a(r,k) = Σ_{j=0..k} A[j] *
 *            A[r-j], * where the sequence A (the first column of...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   ensure_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_len.h"

/*  Forward declaration of seq_t and idx_t are supplied by the build.  */
extern seq_t *alloc_seq_array(size_t n);
extern void free_seq_array(seq_t *p);

static seq_t *A = NULL;        /* first column values A[0..]          */
static seq_t *B = NULL;        /* diagonal values B[0..] (B[0] = 1) */
static size_t Alen = 0;        /* allocated length of A             */
static size_t Blen = 0;        /* allocated length of B             */
static idx_t  last_n = 0;      /* highest n for which A[n] is known  */

/*  Expand A and B up to at least r (so that A[r] and B[r+1] exist).  */
static void expand_to_idx(idx_t r)
{
    /* B must have space for index r+1, A for r.  */
    ensure_len(&B, &Blen, (size_t)(r + 2), &Blen);
    ensure_len(&A, &Alen, (size_t)(r + 1), &Alen);

    /* Initialisation on first call.  */
    if (last_n == 0) {
        B[0] = (seq_t)1;      /* artificial but convenient          */
        B[1] = (seq_t)1;      /* a(0,0) = 1                          */
        A[0] = (seq_t)1;      /* a(0,0) = 1                          */
        /* Now compute A[1] using the relation A[1] = B[1] + B[0] */
        A[1] = B[1] + B[0];
        last_n = 1;
    }

    /* Continuously extend until last_n >= r.  */
    while (last_n < r) {
        idx_t i = last_n;                /* current index for which A[i] exists   */
        /* Compute B[i+1] = Σ_{j=0..i} A[j]*A[i-j]  */
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= i; ++j) {
            sum += A[j] * A[i - j];
        }
        B[i + 1] = sum;
        /* Compute A[i+1] = B[i+1] + B[i] */
        A[i + 1] = B[i + 1] + B[i];
        last_n = i + 1;
    }
}

/*  Find row r and column k for the given flattened index n.  */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    /* Compute the largest r such that r*(r+1)/2 ≤ n.  */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/*  Entry point for the sequence A073154.  */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t r, k;
    index_to_row_col(n, &r, &k);
    /* Ensure A and B are ready for this row.  */
    expand_to_idx(r);

    /* a(r,k) = Σ_{j=0..k} A[j] * A[r-j]    */
    for (idx_t j = 0; j <= k; ++j) {
        result += A[j] * A[r - j];
    }
    return result;
}
