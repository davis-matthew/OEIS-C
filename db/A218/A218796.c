/*
 * A218796  Triangular array read by rows: T(n,k) is the number of
 *          compositions of n that have exactly k 3's; n>=0, 0<=k<=floor(n/3).
 * https://oeis.org/A218796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 69, u32 n <= 223, u64 n <= 807,
 *            u128 n <= 2976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   T(n,k) = sum_{i>=1, i!=3} T(n-i,k) + (k>0?T(n-3,k-1):0)  for n>0,
   T(0,0)=1, others 0.  
   a(n) is obtained by listing T(n,k) row‑wise, i.e. a = T(0,0), T(1,0), T(2,0), T(3,0),T(3,1), ... */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t **T = NULL;          /* arrays of rows */
    static seq_t *sums = NULL;        /* cumulative sums per k */
    static seq_t *flatten = NULL;     /* flattened sequence a(n) */
    static size_t T_rows = 0;         /* number of rows stored in T          */
    static size_t sums_cap = 0;       /* capacity of sums array             */
    static size_t fcap = 0;           /* capacity of flatten array          */
    static size_t a_size = 0;         /* number of elements in flatten      */

    /* initialise on first call */
    if (T_rows == 0 && a_size == 0) {
        /* allocate minimal capacities */
        T = NULL;
        sums = NULL;
        flatten = NULL;
    }

    /* generate rows until we have a[n] computed */
    while (a_size <= n) {
        size_t r = T_rows;           /* next row index to compute */
        size_t kmax = r / 3;         /* maximum k for this row */

        /* ensure sums array large enough */
        if (sums_cap < kmax + 1) {
            size_t new_cap = kmax + 1;
            seq_t *tmp = (seq_t*)realloc(sums, new_cap * sizeof(seq_t));
            sums = tmp;
            /* initialise new slots to zero */
            for (size_t i = sums_cap; i < new_cap; ++i) sums[i] = 0;
            sums_cap = new_cap;
        }

        /* allocate row array */
        seq_t *row = (seq_t*)calloc(kmax + 1, sizeof(seq_t));

        if (r == 0) {
            /* special base case */
            row[0] = (seq_t)1;
            sums[0] += row[0];
        } else {
            /* general recurrence */
            for (size_t k = 0; k <= kmax; ++k) {
                seq_t part = sums[k];

                /* subtract contributions from T[r-3][k] if defined */
                if (r >= 3 && k <= (r - 3) / 3) {
                    part -= T[r - 3][k];
                }
                /* add contributions from T[r-3][k-1] if defined */
                if (k > 0 && r >= 3 && (k - 1) <= (r - 3) / 3) {
                    part += T[r - 3][k - 1];
                }

                row[k] = part;
                sums[k] += part;
            }
        }

        /* extend T array */
        seq_t **tmp_T = (seq_t**)realloc(T, (T_rows + 1) * sizeof(seq_t*));
        T = tmp_T;
        T[T_rows] = row;
        T_rows++;

        /* append to flattened sequence */
        for (size_t k = 0; k <= kmax; ++k) {
            if (a_size == fcap) {
                size_t new_fcap = (fcap == 0) ? 1 : fcap * 2;
                seq_t *tmp_F = (seq_t*)realloc(flatten, new_fcap * sizeof(seq_t));
                flatten = tmp_F;
                fcap = new_fcap;
            }
            flatten[a_size++] = row[k];
        }
    }

    return flatten[(size_t)n];
}
