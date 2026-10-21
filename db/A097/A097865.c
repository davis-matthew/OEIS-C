/*
 * A097865  The n-th group (n>=0) of 9 consecutive terms are the entries, read
 *          by rows, of the 3 X 3 matrix A[n], where A[0]=
 *          [[0,1,1],[1,1,2],[1,2,4]], A[1]=[[1,1,2],[1,1,2],[2,4,7]] and
 *          A[n]=A[n-A[n-1](1,1)] + A[n-A[n-2](1,2)], (M(i,j) denotes the
 *          (i,j)-entry of the matrix M).
 * https://oeis.org/A097865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is defined by the 3×3 matrices A[n].
   A[0] = [[0,1,1],[1,1,2],[1,2,4]]
   A[1] = [[1,1,2],[1,1,2],[2,4,7]]
   A[n] = A[n‑A[n‑1](1,1)] + A[n‑A[n‑2](1,2)]   for n≥2
   The sequence a(n) is the row‑major entries of these matrices. */

#include <stdlib.h>
#include <stdint.h>

/* row‑major order: indices 0..8 correspond to (1,1)…(3,3) */
typedef struct {
    seq_t m[9];
} Matrix;

/* cached matrices */
static Matrix *mat_arr = NULL;   /* array of computed matrices */
static size_t  mat_cap = 0;      /* allocated capacity */
static size_t  mat_size = 0;      /* number of matrices stored */

/* Ensure that matrices[0..limit] are computed and stored. */
static void ensure_size(size_t limit)
{
    if (limit < mat_size)
        return;

    /* Grow the allocation to at least (limit+1). */
    size_t new_cap = mat_cap ? mat_cap << 1 : 2;
    while (new_cap <= limit) new_cap <<= 1;
    Matrix *tmp = (Matrix *)realloc(mat_arr, new_cap * sizeof(Matrix));
    mat_arr = tmp;
    mat_cap = new_cap;

    /* Compute missing matrices sequentially. */
    for (size_t i = mat_size; i <= limit; ++i) {
        if (i == 0) {
            mat_arr[i].m[0] = 0; mat_arr[i].m[1] = 1; mat_arr[i].m[2] = 1;
            mat_arr[i].m[3] = 1; mat_arr[i].m[4] = 1; mat_arr[i].m[5] = 2;
            mat_arr[i].m[6] = 1; mat_arr[i].m[7] = 2; mat_arr[i].m[8] = 4;
        } else if (i == 1) {
            mat_arr[i].m[0] = 1; mat_arr[i].m[1] = 1; mat_arr[i].m[2] = 2;
            mat_arr[i].m[3] = 1; mat_arr[i].m[4] = 1; mat_arr[i].m[5] = 2;
            mat_arr[i].m[6] = 2; mat_arr[i].m[7] = 4; mat_arr[i].m[8] = 7;
        } else {
            /* Determine indices of the two contributing matrices */
            idx_t  val1 = (idx_t)mat_arr[i-1].m[0]; /* A[n-1](1,1) */
            idx_t  val2 = (idx_t)mat_arr[i-2].m[1]; /* A[n-2](1,2) */
            size_t idx1 = (size_t)(i - val1);
            size_t idx2 = (size_t)(i - val2);
            for (int k = 0; k < 9; ++k)
                mat_arr[i].m[k] = mat_arr[idx1].m[k] + mat_arr[idx2].m[k];
        }
    }
    mat_size = limit + 1;
}

/* Public sequence element function. */
seq_t a(idx_t n)
{
    size_t group = (size_t)(n / 9);      /* which 3×3 matrix contains a(n) */
    ensure_size(group);
    size_t pos   = (size_t)(n % 9);      /* position within the matrix */
    return mat_arr[group].m[pos];
}
