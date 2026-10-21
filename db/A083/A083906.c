/*
 * A083906  Table read by rows: T(n, k) is the number of length n binary words
 *          with exactly k inversions.
 * https://oeis.org/A083906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of q^k in Sum_{m=0..n} [n,m]_q, with table
 *            read by rows. The values can be computed by the recurrence
 *            T(n,k) = 2*T(n-1,k) - T(n-2,k) + T(n-2,k-n+1) for n >= 2 and 0
 *            <= k <= floor(n^2/4), with base rows T(0,0) = 1, T(1,0) = 2. Row
 *            n contains floor(n^2/4)+1 entries. The function...
 * Range:     u8 n <= 185, u16 n <= 889, u32 n <= 4943, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Dynamic storage for the table. */
static seq_t **rows   = NULL;     /* pointers to each row            */
static size_t *len    = NULL;     /* length of each row              */
static size_t  rows_cap = 0;      /* allocated capacity of rows      */
static size_t  rows_n   = 0;      /* number of rows currently stored */
static idx_t   total   = 0;       /* total number of terms already built */

/* Ensure that we have capacity for at least 'need' rows. */
static void ensure_rows_capacity(size_t need)
{
    if (need <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap << 1 : 4;
    if (new_cap < need) new_cap = need;
    rows   = (seq_t **)realloc(rows,   new_cap * sizeof(seq_t *));
    len    = (size_t *)realloc(len,    new_cap * sizeof(size_t));
    rows_cap = new_cap;
}

/* Add the next row (index = rows_n) to the table. */
static void add_next_row(void)
{
    idx_t n = (idx_t)rows_n;                /* row index to be added   */
    idx_t L = ((idx_t)n * (idx_t)n) / 4 + 1;/* length of the new row   */
    seq_t *row = (seq_t *)malloc((size_t)L * sizeof(seq_t));
    if (!row) exit(1);                      /* out of memory            */

    if (n == 0) {
        row[0] = (seq_t)1;
    } else if (n == 1) {
        row[0] = (seq_t)2;
    } else {
        /* recurrence uses previous two rows */
        size_t len_nm1 = len[n-1];
        size_t len_nm2 = len[n-2];
        for (idx_t k = 0; k < L; ++k) {
            seq_t a1 = 0, a2 = 0, a3 = 0;
            if (k < (idx_t)len_nm1) a1 = rows[n-1][k];
            if (k < (idx_t)len_nm2) a2 = rows[n-2][k];
            if (k + 1 >= n) {                /* k >= n-1  */
                idx_t idx2 = k - (n - 1);
                if (idx2 < (idx_t)len_nm2) a3 = rows[n-2][idx2];
            }
            seq_t tmp = a1 + a1;             /* 2*a1            */
            tmp += a3;
            row[k] = tmp - a2;               /* tmp - a2         */
        }
    }

    ensure_rows_capacity(rows_n + 1);
    rows[rows_n] = row;
    len[rows_n]  = (size_t)L;
    rows_n++;
    total += L;
}

/* Main function required by the sequence interface. */
seq_t a(idx_t n)
{
    /* Build rows until the requested linear index is inside the table. */
    while (total <= n) add_next_row();

    /* Locate the row that contains the index n. */
    idx_t cum = 0;
    size_t r = 0;
    while (r < rows_n) {
        if (cum + len[r] > n) break;
        cum += (idx_t)len[r];
        ++r;
    }

    idx_t k = n - cum;                    /* column within the row   */
    return rows[r][k];
}
