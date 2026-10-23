/*
 * A008307  Table T(n,k) giving number of permutations of [1..n] with order
 *          dividing k, read by antidiagonals.
 * https://oeis.org/A008307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, look, nice
 * Formula:   a(n) is the reading by antidiagonals of the table T(n,k) giving
 *            the number of permutations of [1..n] whose order divides k. The
 *            table is defined by: T(1,k) = 1 (k ≥ 1) T(n,k) = Σ_{d|k}
 *            (n-1)_{d-1} * T(n-d, k) for n ≥ 2 where (m)_i =
 *            m·(m-1)·…·(m-i+1) is the falling factorial
 * Range:     u8 n <= 37, u16 n <= 79, u32 n <= 156, u64 n <= 381,
 *            u128 n <= 951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types supplied by the build system. */
#include <stdlib.h>
#include <stddef.h>

/* Static storage for the table. */
static seq_t *table = NULL;      /* flattened 2‑D: row * stride + col */
static idx_t  cur_nmax = 0;      /* maximum row stored */
static idx_t  cur_kmax = 0;      /* maximum column stored (≤ stride-1) */
static idx_t  stride   = 1;      /* stride = cur_kmax + 1 */

/* Find the (row,col) pair that corresponds to position n in the antidiagonal
   reading of the table.  Row and col are 1‑based.  The order inside each
   antidiagonal is decreasing row: (row,col) = (S-1),(S-2), …, (1,S-1),
   where S satisfies S*(S-1)/2 ≥ n. */
static void antidiagonal_coords(idx_t pos, idx_t *row, idx_t *col)
{
    /* Binary search for smallest S such that S*(S-1)/2 ≥ pos. */
    idx_t lo = 2, hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < pos)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t S = lo;

    /* Number of pairs before this antidiagonal. */
    idx_t prev_total = (S - 1) * (S - 2) / 2;

    /* Zero‑based offset inside the antidiagonal. */
    idx_t offset = pos - prev_total - 1;

    /* Order inside the antidiagonal is decreasing row. */
    *col = offset + 1;          /* k ranges 1 … S-1 */
    *row = S - *col;            /* n ranges 1 … S-1 */
}

/* Recompute the table so that it contains all rows up to need_n and
   columns up to need_k.  The table is rebuilt from scratch because the
   dimensions are very small (≤ ~150 × 150). */
static void ensure_table(idx_t need_n, idx_t need_k)
{
    if (need_n <= cur_nmax && need_k <= cur_kmax)
        return;      /* already sufficient */

    idx_t new_nmax = need_n > cur_nmax ? need_n : cur_nmax;
    idx_t new_kmax = need_k > cur_kmax ? need_k : cur_kmax;
    idx_t new_stride = new_kmax + 1;
    size_t new_size = (size_t)(new_nmax + 1) * (size_t)new_stride;

    seq_t *new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_table)
        abort();
    /* Initialize to zero. */
    memset(new_table, 0, new_size * sizeof(seq_t));

    /* Compute all required entries. */
    for (idx_t n = 1; n <= new_nmax; ++n) {
        for (idx_t k = 1; k <= new_kmax; ++k) {
            size_t idx = (size_t)n * (size_t)new_stride + (size_t)k;

            if (n == 1) {
                /* Base case: T(1,k) = 1. */
                new_table[idx] = (seq_t)1;
            } else {
                seq_t sum = 0;

                for (idx_t d = 1; d <= k; ++d) {
                    if (k % d != 0)
                        continue;
                    /* The product (n-1)_{d-1}.  Skip if d > n. */
                    if (d > n)
                        continue;

                    seq_t ff = 1;
                    for (idx_t t = 0; t < d - 1; ++t)
                        ff = ff * (seq_t)((n - 1) - t);

                    idx_t prev_n = n - d;      /* n-d ≥ 0 */
                    seq_t prev_val = (prev_n == 0) ? (seq_t)1
                                                   : new_table[(size_t)prev_n * (size_t)new_stride + (size_t)k];

                    sum = sum + prev_val * ff;
                }

                new_table[idx] = sum;
            }
        }
    }

    /* Replace old table. */
    free(table);
    table   = new_table;
    stride  = new_stride;
    cur_nmax = new_nmax;
    cur_kmax = new_kmax;
}

/* Sequence entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined indices return 0 */

    idx_t row, col;
    antidiagonal_coords(n, &row, &col);

    ensure_table(row, col);

    size_t idx = (size_t)row * (size_t)stride + (size_t)col;
    return table[idx];
}
