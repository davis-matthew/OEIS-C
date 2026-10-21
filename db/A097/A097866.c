/*
 * A097866  The n-th group (n>=0) of 9 consecutive terms are the entries, read
 *          by rows, of the 3 X 3 matrix A[n], where A[0]= [[0, 1, 1], [1, 1,
 *          2], [1, 2, 4]], A[1]=[[1, 1, 2], [1, 1, 2], [2, 4, 7]], A[2]=[[1,
 *          1, 2], [1, 1, 2], [4, 6, 11]] and A[n]=A[abs(n-A[n-1](1, 1))] +
 *          A[abs(n-A[n-2](1, 2))] + A[abs(n-A...
 * https://oeis.org/A097866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   abs_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A097866 – 3×3 matrix recurrence
 *
 * a(9k+j) = A[k](row=j/3+1, col=j%3+1) (0‑based indexing).
 * A[0] = [[0,1,1],[1,1,2],[1,2,4]]
 * A[1] = [[1,1,2],[1,1,2],[2,4,7]]
 * A[2] = [[1,1,2],[1,1,2],[4,6,11]]
 * For n ≥ 3:
 *   A[n] = A[|n – A[n‑1](1,1)|] + A[|n – A[n‑2](1,2)|] + A[|n – A[n‑3](1,3)|]
 *
 * The implementation memoises the matrices; each matrix is computed only
 * once, regardless of the order of calls, and each is derived using only
 * integer arithmetic on seq_t and idx_t.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/abs_idx.h"

typedef struct {
    seq_t m[9];
} Matrix;

/* Arrays of computed matrices and flags */
static Matrix *mats      = NULL;
static bool   *computed  = NULL;
static size_t  mats_cap  = 0;

/* Ensure the arrays can index up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < mats_cap) return;
    size_t new_cap = mats_cap ? mats_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;
    /* reallocate Matrix array */
    Matrix *tmpM = (Matrix *)realloc(mats, new_cap * sizeof(Matrix));
    bool   *tmpC = (bool *)realloc(computed, new_cap * sizeof(bool));
    if (!tmpM || !tmpC) abort();
    mats     = tmpM;
    computed = tmpC;
    /* initialise new slots */
    memset(&mats[mats_cap], 0, (new_cap - mats_cap) * sizeof(Matrix));
    memset(&computed[mats_cap], 0, (new_cap - mats_cap) * sizeof(bool));
    mats_cap = new_cap;
}

/* Build all matrices from the smallest missing one up to n */
static void compute_matrix(idx_t n)
{
    if ((size_t)n >= mats_cap) ensure_capacity(n);
    if (computed[n]) return;

    /* Find the first uncomputed index */
    idx_t start = 0;
    while (start < mats_cap && computed[start]) start++;

    /* Compute missing matrices in ascending order */
    for (idx_t i = start; i <= n; ++i) {
        if (i == 0) {
            static const seq_t a0[9] = {0,1,1, 1,1,2, 1,2,4};
            memcpy(mats[0].m, a0, sizeof(a0));
        } else if (i == 1) {
            static const seq_t a1[9] = {1,1,2, 1,1,2, 2,4,7};
            memcpy(mats[1].m, a1, sizeof(a1));
        } else if (i == 2) {
            static const seq_t a2[9] = {1,1,2, 1,1,2, 4,6,11};
            memcpy(mats[2].m, a2, sizeof(a2));
        } else {
            idx_t idx1 = abs_idx(i, mats[i-1].m[0]);   /* A[i-1](1,1) */
            idx_t idx2 = abs_idx(i, mats[i-2].m[1]);   /* A[i-2](1,2) */
            idx_t idx3 = abs_idx(i, mats[i-3].m[2]);   /* A[i-3](1,3) */

            /* Dependencies are guaranteed computed because i-1,i-2,i-3 < i */
            for (int k = 0; k < 9; ++k)
                mats[i].m[k] = mats[idx1].m[k] + mats[idx2].m[k] + mats[idx3].m[k];
        }
        computed[i] = true;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    idx_t group = n / 9;
    idx_t pos   = n % 9;
    compute_matrix(group);
    return mats[group].m[pos];
}
