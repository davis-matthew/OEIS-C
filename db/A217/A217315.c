/*
 * A217315  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >= 1 or
 *          if k-n >= 8, T(0,k)= 1 if 0<=k<=7, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A217315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A217315 – Square array read by antidiagonals.
    T(n,k) = 0 if n > k or k - n >= 8,
    T(0,k) = 1 for 0 ≤ k ≤ 7, 0 otherwise.
    For n > 0, T(n,k) = T(n-1,k) + T(n,k-1).

    a(n) is obtained by traversing the array along antidiagonals
    (n + k = const) and listing the values in order.
    a(0) = T(0,0), a(1) = T(0,1), a(2) = T(1,0), etc. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *seq          = NULL;
    static size_t seq_len      = 0;   /* number of stored terms          */
    static size_t seq_cap      = 0;   /* current capacity of seq array   */

    /* Cached table of rows: table[i] points to an array of 8 seq_t values,
       representing T(i, i+d) for d = 0..7. */
    static seq_t **table       = NULL;
    static size_t rows         = 0;   /* number of rows currently stored*/

    /* Next antidiagonal to process (s = n + k). */
    static idx_t next_diag     = 0;

    /* Extend the sequence until we have at least n+1 terms. */
    while (seq_len <= n)
    {
        /* Make sure all rows up to next_diag exist. */
        while (rows <= next_diag)
        {
            idx_t r = rows;                  /* new row index to create   */
            seq_t *row = (seq_t *)malloc(sizeof(seq_t) * 8);
            if (!row)
                return (seq_t)0;              /* memory failure */

            if (r == 0)                       /* base row T(0,k) = 1 for k<=7 */
            {
                for (int d = 0; d < 8; ++d)
                    row[d] = (seq_t)1;
            }
            else                              /* compute row r from row r-1 */
            {
                for (int d = 0; d < 8; ++d) {
                    seq_t above = (d + 1 <= 7) ? table[r-1][d+1] : (seq_t)0;
                    seq_t left  = (d > 0) ? row[d-1] : (seq_t)0;
                    row[d] = above + left;
                }
            }

            /* Grow the table array itself if needed. */
            seq_t **new_tbl = (seq_t **)realloc(
                table, (rows + 1) * sizeof(seq_t *));
            if (!new_tbl) {
                free(row);
                return (seq_t)0;              /* memory failure */
            }
            table = new_tbl;

            table[rows] = row;
            ++rows;                           /* one more row stored   */
        }

        /* Process antidiagonal 'next_diag' (s). */
        idx_t s = next_diag;
        for (idx_t i = 0; i <= s; ++i)
        {
            /* k = s - i; d = k - i = s - 2*i. */
            long long d = (long long)s - 2LL * (long long)i;
            seq_t val = 0;
            if (d >= 0 && d <= 7)
                val = table[i][(int)d];

            /* Append the value to the sequence array. */
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 1;
                seq_t *new_seq = (seq_t *)realloc(
                    seq, seq_cap * sizeof(seq_t));
                if (!new_seq)
                    return (seq_t)0;          /* memory failure */
                seq = new_seq;
            }
            seq[seq_len++] = val;
        }

        ++next_diag;
    }

    return seq[n];
}
