/*
 * A210950  Triangle read by rows: T(n,k) = number of parts in the k-th column
 *          of the partitions of n but with the partitions aligned to the
 *          right margin.
 * https://oeis.org/A210950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210950 – Triangle read by rows.
   The triangle entry T(n,k) equals the number of partitions of n
   into at least n-k+1 parts.  The sequence a(n) is that triangle
   read row by row:  a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), ...  */

/* Standard headers needed for both C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* The public function required by the build system. */
seq_t a(idx_t n)
{
    /* --------------  Map sequence index to (row, column)  -------------- */
    if (n == 0) return 0;

    /* Compute the row r such that r(r-1)/2 < n ≤ r(r+1)/2.
       Use a floating point estimate and then correct it.         */
    long double nd = (long double)n;
    idx_t r = (idx_t)((sqrtl((long double)8 * nd + 1.0L) - 1.0L) / 2.0L);
    /* Adjust in case the estimate is off by one.                    */
    while ((r * (r + 1)) / 2 < n) ++r;
    while ((r > 0) && ((r - 1) * r / 2) >= n) --r;

    /* Column number (1‑based) within this row.                       */
    idx_t prev = (r - 1) * r / 2;
    idx_t col  = n - prev;               /* 1 ≤ col ≤ r */

    /* --------------  Partition table up to the needed row  -------------- */
    /* p[i][j] will hold the number of partitions of i into exactly j parts. */
    /* The table is stored as a jagged array;  p[0] is a single element 1.   */
    static seq_t **p = NULL;
    static idx_t max_row = 0;  /* highest row that has already been created   */

    /* Allocate p[0] if not yet initialized.                           */
    if (max_row == 0) {
        p = (seq_t **)malloc(sizeof(seq_t *));
        if (!p) return 0;              /* allocation failure – return 0          */
        p[0] = (seq_t *)calloc(1, sizeof(seq_t));
        if (!p[0]) { free(p); return 0; }
        p[0][0] = (seq_t)1;            /* one way to partition 0               */
    }

    /* Grow the triangle up to row r if necessary.                       */
    if (r > max_row) {
        seq_t **new_p = (seq_t **)realloc(p, (r + 1) * sizeof(seq_t *));
        if (!new_p) return 0;         /* allocation failure – return 0         */
        p = new_p;

        /* Build each new row i > max_row.                                */
        for (idx_t i = max_row + 1; i <= r; ++i) {
            p[i] = (seq_t *)calloc((i + 1), sizeof(seq_t));
            if (!p[i]) return 0;      /* allocation failure – return 0         */

            for (idx_t j = 1; j <= i; ++j) {
                seq_t val = 0;

                /* Partitions that decrease the number of parts by one. */
                val += p[i - 1][j - 1];

                /* Partitions that decrease each part by one (if possible). */
                if (i >= 2 * j)          /* i - j >= j  <=>  i >= 2*j          */
                    val += p[i - j][j];

                p[i][j] = val;
            }
        }
        max_row = r;                    /* Remember highest computed row         */
    }

    /* --------------  Compute T(row, col)  -------------- */
    /* The minimum number of parts is r - col + 1.            */
    idx_t min_parts = r - col + 1;
    seq_t sum = (seq_t)0;
    for (idx_t j = min_parts; j <= r; ++j) {
        sum += p[r][j];
    }
    return sum;
}
