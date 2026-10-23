/*
 * A165997  Irregular triangle read by rows: T(0,0) = 1, T(n,k) = T(n,k-1) +
 *          T(n-1,k) for n > 0, 0 < k <= f(n), where f(n) = floor((2*n+3)/3),
 *          and entries outside triangle are 0.
 * https://oeis.org/A165997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is read row by row from the irregular triangle defined by *
 *            T(0,0) = 1, and for n > 0, 0 < k <= floor((2*n+3)/3), * T(n,k) =
 *            T(n,k-1) + T(n-1,k). * Row n contains floor((2*n+3)/3) elements
 *            (indices 1..floor((2*n+3)/3)). * Adding the entries of the rows
 *            in order yields the sequence a(0),a(1),…
 * Range:     u8 n <= 34, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the very first term */
    if (n == 0)
        return (seq_t)1;

    /* cumulative number of terms processed so far */
    size_t cumulative = 1;          /* row 0 contributes one term */
    size_t r = 1;                   /* start from row 1 */
    seq_t *prev = NULL;             /* previous row values (1‑based indices) */
    size_t prev_len = 0;            /* length of previous row = floor((2*(r-1)+3)/3) */

    size_t target_row = 0;          /* the row that contains a(n) */
    size_t target_index = 0;        /* 0‑based index within that row */

    /* Find the row that contains the n‑th term */
    while (1) {
        size_t len = (2*r + 3) / 3;          /* number of elements in row r */
        if (cumulative + len > n) {
            target_row   = r;
            target_index = (size_t)(n - cumulative);
            break;
        }

        /* compute the full row r and store it for the next iteration */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();                   /* allocation failed */

        row[0] = (seq_t)1;
        for (size_t k = 1; k < len; ++k) {
            seq_t add = (k < prev_len) ? prev[k] : (seq_t)0;
            row[k] = row[k-1] + add;
        }

        /* discard older row */
        if (prev) free(prev);
        prev    = row;
        prev_len = len;

        cumulative += len;
        ++r;
    }

    /* Compute the requested entry, possibly using the cached previous row */
    seq_t result = (seq_t)1;
    if (target_row > 1) {
        /* use prev (row target_row-1) */
        for (size_t k = 1; k <= target_index; ++k) {
            seq_t add = (k < prev_len) ? prev[k] : (seq_t)0;
            result += add;
        }
        free(prev);
    }
    /* For target_row == 1 the first element is always 1; result already set. */
    return result;
}
