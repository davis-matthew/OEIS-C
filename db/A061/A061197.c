/*
 * A061197  Table by antidiagonals T(n,k) of number of partitions of k where
 *          the largest part is less than or equal to n and where there are no
 *          more than two of any particular sized part.
 * https://oeis.org/A061197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   build_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061197:  sequence of partitions of k with part–size ≤ n and at most two of each size.
   Antidiagonal enumeration gives a(n) = T(i,j) where i+j = s and
   n = s(s+1)/2 + i.

   Recurrence:
      T(0,0) = 1,  T(n,k) = 0 for n<0 or k<0
      T(n,k) = T(n-1,k) + T(n-1,k-n) + T(n-1,k-2*n)
*/

#include <stdlib.h>
#include "../utils/build_table.h"

static seq_t *table = NULL;          /* flat 2‑D array T[n][k] */
static idx_t  table_size = 0;        /* number of rows that have been built */

 

/* return the smallest s such that n < (s+1)(s+2)/2   (i.e. n lies in diagonal s) */
static idx_t find_s(idx_t n)
{
    idx_t s = 0;
    /* use 128‑bit arithmetic to avoid overflow when s is large */
    while ( ((unsigned __int128)(s+1)*(s+2))/2 <= n ) ++s;
    return s;
}

/* ensure that table is built for all rows up to s (including s) */
static void ensure_table(idx_t s)
{
    if (table == NULL || s > table_size)
    {
        seq_t *new_table = build_table(s);
        if (table) free(table);
        table      = new_table;
        table_size = s;
    }
}

seq_t a(idx_t n)
{
    /* locate antidiagonal coordinates (i,j) for the n‑th term */
    idx_t s = find_s(n);                          /* diagonal index */
    ensure_table(s);                              /* build table up to n so we have all needed T(i,j) */

    /* prefix length of all previous diagonals */
    idx_t prefix = (idx_t)(((unsigned __int128)s*(s+1))/2);
    idx_t i      = n - prefix;                    /* row index on diagonal */
    idx_t j      = s - i;                         /* column index on diagonal */

    /* look up the value in the pre‑computed table */
    return table[i*(table_size+1) + j];
}
