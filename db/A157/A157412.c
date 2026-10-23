/*
 * A157412  Triangular read by rows: T(n,m) = J(prime(n),prime(m)) where J is
 *          the Jacobi symbol. Each row starts with prime(2) = 3.
 * https://oeis.org/A157412
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = Jacobi( prime(r), prime(c) ) where (r,c) are the row and
 *            column indices of the triangle read by rows, starting at
 *            prime(2)=3. Row r has length r‑1 and contains T(r,2)…T(r,r). The
 *            mapping from n (with offset 2) to (r,c) is performed by the
 *            helper below
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

static void row_col_from_index(idx_t idx, idx_t *row, idx_t *col)
{
    /* idx is the zero‑based offset from the first element (n‑2).  */
    uint64_t L = (uint64_t)idx;

    /* Find largest m such that m*(m+1)/2 <= L  (m = r‑2).  */
    uint64_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= L) high <<= 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= L) low = mid;
        else high = mid - 1;
    }

    uint64_t m   = low;              /* m = r‑2                            */
    uint64_t r   = m + 2;            /* row index                           */
    uint64_t start = m * (m + 1) / 2;/* first offset of this row             */
    uint64_t c0  = L - start;        /* zero‑based column inside the row    */
    uint64_t c   = 2 + c0;           /* column index (prime index)          */

    *row = (idx_t)r;
    *col = (idx_t)c;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* below offset – undefined behavior */

    idx_t row, col;
    row_col_from_index(n - 2, &row, &col);

    /* Retrieve the primes for the given indices.  */
    uint64_t p_row = (uint64_t)get_prime(row);
    uint64_t p_col = (uint64_t)get_prime(col);

    /* Compute the Jacobi symbol J(p_row, p_col).  */
    int j = jacobi_symbol(p_row, p_col);

    return (seq_t)j;
}
