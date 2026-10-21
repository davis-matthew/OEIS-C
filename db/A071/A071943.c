/*
 * A071943  Triangle T(n,k) (n>=0, 0 <= k <= n) read by rows giving number of
 *          underdiagonal lattice paths from (0,0) to (n,k) using only steps
 *          R=(1,0), V=(0,1) and D=(1,2).
 * https://oeis.org/A071943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 63, u32 n <= 186, u64 n <= 558,
 *            u128 n <= 1950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071943 – Triangle T(n,k) read by rows.
 *  Recurrence:  T(n,0)=1;
 *                T(n,k)=T(n,k-1)+T(n-1,k)+T(n-1,k-2)   (k>0),
 *                T(n,k)=0 for k<0 or k>n.
 *  a(n) is the n‑th term of the flattened triangle (row‑major order). */
#include <stddef.h>

/* MAX_ROW is large enough for n up to ~20000 (rows up to ~200). */
#define MAX_ROW 200

seq_t a(idx_t n)
{
    /*  Find the row r (0‑based) in which index n lies.
     *  The number of terms up to (and including) row r is
     *  (r+1)(r+2)/2.  We need the smallest r with that value > n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /*  Column k in that row (0‑based). */
    idx_t k = n - (r * (r + 1) / 2);

    /*  If we only need the very first row, return immediately. */
    if (r == 0) {
        return (k == 0) ? (seq_t)1 : (seq_t)0;
    }

    /*  Allocate two rows on the stack; size MAX_ROW is more than enough. */
    seq_t prev[MAX_ROW] = {0};
    seq_t cur[MAX_ROW]  = {0};

    /*  Initialise row 0. */
    prev[0] = 1;

    /*  Build rows 1 .. r, reusing the two arrays. */
    for (idx_t i = 1; i <= r; ++i) {
        cur[0] = 1;                      /* T(i,0)=1 */

        for (idx_t j = 1; j <= i; ++j) {
            seq_t val = cur[j - 1];       /* T(i,j-1) */

            if (j < i)                   /* T(i-1,j) */
                val += prev[j];

            if (j >= 2)                   /* T(i-1,j-2) */
                val += prev[j - 2];

            cur[j] = val;
        }

        /*  Copy the current row into prev for the next iteration. */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = cur[j];
    }

    /*  At this point prev holds row r; return the k‑th element. */
    return prev[k];
}
