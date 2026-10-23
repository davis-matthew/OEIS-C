/*
 * A049837  Triangular array T read by rows: T(n,k)=number of distinct numbers
 *          that appear when Euclidean algorithm acts on n,k, for k=1,2,...,n;
 *          n=1,2,3,...
 * https://oeis.org/A049837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of distinct numbers that appear (including the
 *            initial n and k, and all quotients and remainders) when the
 *            Euclidean algorithm is applied to the pair (n,k). The sequence
 *            is obtained by reading the triangular array T(n,k) row‑by‑row;
 *            i.e. a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the term a(n) (n ≥ 1) as described above. */
seq_t a(idx_t n)
{
    /* Find the row `row` and column `col` corresponding to index `n`
       in the row‑major enumeration of the triangular array.        */
    idx_t row = 1;
    unsigned __int128 tri = (unsigned __int128)row * (row + 1) / 2;
    while (tri < n) {
        ++row;
        tri = (unsigned __int128)row * (row + 1) / 2;
    }
    /* Number of terms preceding this row: (row-1)*row/2 */
    unsigned __int128 prev_tri = (unsigned __int128)(row - 1) * row / 2;
    idx_t col = (idx_t)(n - prev_tri);          /* 1‑based column */

    /* Allocate a boolean array to mark numbers seen in this run.
       All numbers that can appear are ≤ `row`; allocate size row+1. */
    size_t sz = (size_t)(row + 1);
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) {
        /* Allocation failure: return 0 (not expected for n ≤ 10000). */
        return 0;
    }

    /* Count distinct numbers.  Use a seq_t counter. */
    seq_t count = 0;

    /* The Euclidean algorithm will involve numbers a=row, b=col.
       All numbers involved (quotients, remainders, and the initial
       values) are ≤ row, so indexing `seen` by the numeric value is safe. */
    idx_t a_cur = row, b_cur = col;
    /* Add the initial values. */
    if (!seen[(size_t)a_cur]) { seen[(size_t)a_cur] = true; ++count; }
    if (!seen[(size_t)b_cur]) { seen[(size_t)b_cur] = true; ++count; }

    while (b_cur != 0) {
        idx_t q = a_cur / b_cur;
        idx_t r = a_cur % b_cur;

        if (!seen[(size_t)q]) { seen[(size_t)q] = true; ++count; }
        if (!seen[(size_t)r]) { seen[(size_t)r] = true; ++count; }

        a_cur = b_cur;
        b_cur = r;
    }

    free(seen);
    return count;
}
