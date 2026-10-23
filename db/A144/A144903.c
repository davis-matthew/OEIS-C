/*
 * A144903  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of x/((1-x-x^3)*(1-x)^(k-1)).
 * https://oeis.org/A144903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(i,j) (n>=0, k>=0)
 *            by antidiagonals. A(0,k)=0, A(n,0) is defined by A(0,0)=0,
 *            A(1,0)=1, A(2,0)=0, A(n,0)=A(n-1,0)+A(n-3,0) (n>=3). For k>=1,
 *            A(n,k) is the cumulative sum of the previous column:
 *            A(n,k)=A(0,k-1)+A(1,k-1)+...+A(n,k-1) (n>=0). Thus we can...
 * Range:     u8 n <= 83, u16 n <= 239, u32 n <= 719, u64 n <= 2515,
 *            u128 n <= 9107, big any n
 * Helpers:   antidiagonal_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/antidiagonal_d.h"

static seq_t *A = NULL;          /* 1‑D table A[i*size + j]                   */
static idx_t  table_size = 0;    /* current size of one dimension of A        */

/* ensure that A has dimensions at least needed x needed */
static void ensure_table(idx_t needed)
{
    if (table_size >= needed)
        return;

    /* double size until sufficient */
    idx_t new_size = table_size ? table_size : 1;
    while (new_size < needed)
        new_size <<= 1;

    /* allocate new table */
    seq_t *newA = (seq_t *)calloc(new_size * new_size, sizeof(seq_t));
    if (!newA) { /* allocation failed; leave old table untouched */
        return;
    }

    /* compute column 0 (k=0) */
    if (new_size > 0) newA[0 * new_size + 0] = 0;
    if (new_size > 1) newA[1 * new_size + 0] = 1;
    if (new_size > 2) newA[2 * new_size + 0] = 0;
    for (idx_t n = 3; n < new_size; ++n) {
        newA[n * new_size + 0] = newA[(n - 1) * new_size + 0] + newA[(n - 3) * new_size + 0];
    }

    /* compute remaining columns by cumulative sums */
    for (idx_t k = 1; k < new_size; ++k) {
        seq_t sum = 0;
        for (idx_t n = 0; n < new_size; ++n) {
            sum += newA[n * new_size + (k - 1)];
            newA[n * new_size + k] = sum;
        }
    }

    /* free old memory and update globals */
    free(A);
    A = newA;
    table_size = new_size;
}

/* required sequence function */
seq_t a(idx_t n)
{
    idx_t d = antidiagonal_d(n);
    idx_t diag_start = d * (d + 1) / 2;          /* integer arithmetic safe for small d */
    idx_t offset = n - diag_start;
    idx_t i = offset;             /* row index   */
    idx_t j = d - offset;         /* column index */

    /* ensure the table covers indices up to d */
    ensure_table(d + 1);

    /* safety check (should never happen) */
    if (i >= table_size || j >= table_size)
        return 0;

    return A[i * table_size + j];
}
