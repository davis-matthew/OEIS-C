/*
 * A238941  Triangle T(n,k), read by rows given by (1, 1, 1, 0, 0, 0, 0, 0, 0,
 *          0, ...) DELTA (1, 0, -1, 0, 0, 0, 0, 0, 0, 0, ...) where DELTA is
 *          the operator defined in A084938.
 * https://oeis.org/A238941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle T(m,k) read in row‑major order, where *
 *            T(n,k) = 3*T(n-1,k) + T(n-2,k-2) - T(n-2,k) with *
 *            T(0,0)=T(1,0)=T(1,1)=1, T(n,k)=0 if k<0 or k>n. * The row index
 *            m is the largest integer with m*(m+1)/2 <= n, * and the column
 *            index k = n - m*(m+1)/2. The function * memoises the fille...
 * Range:     u8 n <= 28, u16 n <= 79, u32 n <= 277, u64 n <= 953,
 *            u128 n <= 3583, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Determine row m and column k from linear index n */
    idx_t m;
    {
        /* Initial estimate using floating point, then correct with integer checks */
        double approx = sqrt((double)8.0 * (double)n + 1.0);
        m = (idx_t)((approx - 1.0) * 0.5);
        /* Correct possible rounding errors */
        while (((unsigned __int128)(m + 1) * (m + 2)) / 2 <= n) ++m;
        while (((unsigned __int128)m * (m + 1)) / 2 > n) --m;
    }
    idx_t k = n - ((idx_t)((unsigned __int128)m * (m + 1) / 2));

    /* Static cache for the triangle, built lazily and expanded as needed */
    static seq_t *tri = NULL;      /* flattened triangle data */
    static idx_t max_row = 0;      /* highest row index computed */
    static bool initialized = false;

    if (!initialized || m > max_row) {
        /* Compute required rows up to m */
        idx_t needed_rows = m;                      /* rows 0..m inclusive */
        idx_t new_total = ((unsigned __int128)needed_rows + 1) * (needed_rows + 2) / 2;
        seq_t *newptr = (seq_t *)realloc(tri, new_total * sizeof(seq_t));
        if (!newptr) abort();
        tri = newptr;
        idx_t start_row = initialized ? max_row + 1 : 0;

        for (idx_t rr = start_row; rr <= needed_rows; ++rr) {
            idx_t offset_rr = rr * (rr + 1) / 2;     /* start index of row rr in flat array */

            if (rr == 0) {                /* base case */
                tri[0] = 1;
            } else if (rr == 1) {          /* base case */
                tri[offset_rr] = 1;        /* T(1,0) */
                tri[offset_rr + 1] = 1;    /* T(1,1) */
            } else {                       /* recurrence for rr >= 2 */
                for (int kk = 0; kk <= (int)rr; ++kk) {
                    seq_t val = 0;

                    if (kk <= rr - 1) {
                        val += 3 * tri[(rr - 1) * rr / 2 + kk];
                    }
                    if (kk >= 2) {
                        val += tri[(rr - 2) * (rr - 1) / 2 + (kk - 2)];
                    }
                    if (kk <= rr - 2) {
                        val -= tri[(rr - 2) * (rr - 1) / 2 + kk];
                    }
                    tri[offset_rr + kk] = val;
                }
            }
        }
        max_row = needed_rows;
        initialized = true;
    }

    /* Retrieve the desired entry */
    idx_t offset = m * (m + 1) / 2;
    return tri[offset + k];
}
