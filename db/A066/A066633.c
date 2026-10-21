/*
 * A066633  Triangle T(n,k), n >= 1, 1 <= k <= n, giving number of k's in all
 *          partitions of n.
 * https://oeis.org/A066633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) enumerates the triangle T(row,col), where T(n,k) =
 *            Σ_{j=1..⌊n/k⌋} P(n−j·k)
 * Range:     u8 n <= 78, u16 n <= 595, u32 n <= 5886, u64 n <= 7825,
 *            u128 n <= 7825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external types provided by the build */
#include <stdlib.h>
#include <math.h>

/* static storage for partition numbers */
static seq_t *partitions = NULL;
static idx_t  part_max    = 0;  /* largest n for which partitions[0..n] is valid */

/* recompute partition numbers up to N (inclusive) */
static void ensure_partitions(idx_t N)
{
    if (N <= part_max)
        return;

    /* allocate/reallocate array */
    seq_t *tmp = (seq_t *)realloc(partitions, (N + 1) * sizeof(seq_t));
    if (!tmp)
        exit(1);                       /* out of memory */
    partitions = tmp;

    /* initialize and compute partitions */
    for (idx_t i = 0; i <= N; ++i)
        partitions[i] = 0;
    partitions[0] = 1;                  /* P(0) = 1 */

    for (idx_t s = 1; s <= N; ++s) {
        for (idx_t t = s; t <= N; ++t) {
            partitions[t] += partitions[t - s];
        }
    }

    part_max = N;
}

/* convert triangular index to (row, col)
   row is the row number (1‑based), col is stored in *col (1‑based) */
static idx_t index_to_row_col(idx_t idx, idx_t *col)
{
    if (idx < 1) {
        *col = 0;
        return 0;
    }

    /* estimate row using the quadratic formula */
    double approx = (sqrt(8.0L * (long double)idx + 1.0L) - 1.0L) * 0.5L;
    idx_t row = (idx_t)approx;

    /* correct the estimate */
    while ((__int128)row * (row + 1) / 2 < idx)
        ++row;
    while (row > 0 && (__int128)(row - 1) * row / 2 >= idx)
        --row;

    idx_t start = 1 + row * (row - 1) / 2;
    *col = idx - start + 1;
    return row;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t col;
    idx_t row = index_to_row_col(n, &col);
    if (row == 0 || col == 0)
        return 0;

    ensure_partitions(row);

    seq_t total = 0;
    /* iterate j = n - col, n - 2·col, … >= 0 */
    long long rem = (long long)row - (long long)col;
    while (rem >= 0) {
        total += partitions[(idx_t)rem];
        rem -= (long long)col;
    }
    return total;
}
