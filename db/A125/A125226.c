/*
 * A125226  Array, read by antidiagonals, where A(1,1) = A(1,2) = A(2,1) =
 *          A(2,2) = 1, A(n,k) = 0 if n<1 or k<1, otherwise A(n,k) =
 *          A(n-2,k-2) + A(n-1,k-2) + A(n-2,k-1) + A(n-1,k-1).
 * https://oeis.org/A125226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(1,1)=A(1,2)=A(2,1)=A(2,2)=1, for n,k≥1
 *            A(n,k)=A(n-2,k-2)+A(n-1,k-2)+A(n-2,k-1)+A(n-1,k-1), and A(n,k)=0
 *            if n<1 or k<1. The sequence a(n) is obtained by reading the
 *            array by anti‑diagonals (i+j constant)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   allocate_table, free_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/allocate_table.h"
#include "../utils/free_table.h"

static seq_t *A_table = NULL;   /* flat array of size (S+1)*(S+1) */
static idx_t  A_sz    = 0;      /* current maximum index S */

/* Build the table up to index S (inclusive) using the recurrence. */
static void build_table(idx_t S)
{
    seq_t *newtbl = allocate_table(S);
    idx_t w = S + 1;

    for (idx_t i = 1; i <= S; ++i) {
        for (idx_t j = 1; j <= S; ++j) {
            seq_t val = 0;
            if ((i == 1 && j == 1) || (i == 1 && j == 2) ||
                (i == 2 && j == 1) || (i == 2 && j == 2)) {
                val = 1;
            } else {
                if (i >= 3 && j >= 3)
                    val += newtbl[(i - 2) * w + (j - 2)];
                if (i >= 2 && j >= 3)
                    val += newtbl[(i - 1) * w + (j - 2)];
                if (i >= 3 && j >= 2)
                    val += newtbl[(i - 2) * w + (j - 1)];
                if (i >= 2 && j >= 2)
                    val += newtbl[(i - 1) * w + (j - 1)];
            }
            newtbl[i * w + j] = val;
        }
    }

    free_table(A_table);
    A_table = newtbl;
    A_sz = S;
}

/* Ensure that the table is built up to at least index S. */
static void ensure_table(idx_t S)
{
    if (S <= A_sz) return;
    build_table(S);
}

/* Return the n‑th term a(n).  Indices are 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    /* Find the anti‑diagonal number d (i+j) such that (d-1)*d/2 >= n. */
    idx_t d = (idx_t)((sqrtl((long double)8 * n + 1) + 1) / 2);
    /* Correct any rounding errors. */
    while (((unsigned __int128)(d - 1) * d) / 2 < n) ++d;
    while (d > 2 && (((unsigned __int128)(d - 2) * (d - 1)) / 2) >= n) --d;

    /* We need matrix indices up to d-1. */
    idx_t need = d - 1;
    ensure_table(need);

    idx_t prev = ((d - 2) * (d - 1)) / 2;      /* elements before this diagonal */
    idx_t offset = n - prev - 1;               /* 0‑based offset on the diagonal */
    idx_t i = offset + 1;                      /* row index (1‑based) */
    idx_t j = d - i;                           /* column index */

    idx_t width = A_sz + 1;
    return A_table[i * width + j];
}
