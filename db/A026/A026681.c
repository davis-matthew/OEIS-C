/*
 * A026681  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0; for n
 *          >= 2 and 1<=k<=n-1, T(n,k)=T(n-1,k-1)+T(n-1,k) if k or n-k is of
 *          form 2h for h=1,2,...,[ n/4 ], else
 *          T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k).
 * https://oeis.org/A026681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 143, u32 n <= 509, u64 n <= 1797,
 *            u128 n <= 6839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026681 – triangular array T read by rows
 *
 *  T(0,0)=1,  T(n,0)=T(n,n)=1  (n≥0)
 *  For n≥2 and 1≤k≤n-1:
 *      let M = 2*⌊n/4⌋
 *      if k even and k≤M  or  (n−k) even and (n−k)≤M
 *          T(n,k)=T(n-1,k-1)+T(n-1,k)
 *      else
 *          T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k)
 *  The sequence a(n) is obtained by reading the rows of T left‑to‑right
 *  starting with a(1)=T(0,0).  This function implements the above
 *  recurrence straight, using small static buffers and no dynamic
 *  allocation.  It works in O(n) time and O(1) additional space
 *  (apart from the small buffers).
 */

#include <stdbool.h>
#include <stddef.h>

/* Maximum row length needed for n up to about 10000:
 *  (n+1)(n+2)/2 ≥ 10000  →  n ≈ 140, so 200 is plenty. */
#define MAX_ROW_LEN 200

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Undefined for offset 1, but safe */

    idx_t target = n - 1;            /* 0‑based flattened index */
    /* Static buffers for three consecutive rows */
    static seq_t rows[3][MAX_ROW_LEN];
    static size_t len[3] = {0, 0, 0};

    /* Initialize row 0 */
    rows[0][0] = 1;
    len[0] = 1;
    size_t idxCurr = 0;     /* index of current row in rows */
    size_t idxPrevPrev = 2; /* index of row n-2 (wrap‑around) */

    idx_t cum = 0;          /* number of entries processed so far */

    /* Handle a(1) directly for speed */
    if (target == 0)
        return rows[0][0];

    while (1) {
        /* Check if target lies in current row */
        if (cum + len[idxCurr] > target) {
            idx_t offset = target - cum;
            return rows[idxCurr][offset];
        }

        /* Advance past current row */
        cum += len[idxCurr];

        /* Prepare next row r+1 */
        size_t lenNext = len[idxCurr] + 1;
        size_t idxNext = (idxCurr + 1) % 3;
        len[idxNext] = lenNext;

        /* New row index n = len[idxCurr] (since len = n+1) */
        idx_t n_new = (idx_t)len[idxCurr];
        /* M = 2*floor(n_new/4) */
        idx_t M = 2 * (n_new / 4);

        /* Fill the new row */
        for (size_t k = 0; k < lenNext; ++k) {
            if (k == 0 || k == lenNext - 1) {
                rows[idxNext][k] = 1;
            } else {
                bool cond = false;
                if ((((k & 1U) == 0) && (k <= M)))
                    cond = true;
                else if ((((n_new - k) & 1U) == 0) && ((n_new - k) <= M))
                    cond = true;

                if (cond) {
                    rows[idxNext][k] =
                        rows[idxCurr][k - 1] + rows[idxCurr][k];
                } else {
                    /* n_new ≥ 2 ensures idxPrevPrev is valid */
                    rows[idxNext][k] = rows[idxCurr][k - 1] +
                                       rows[idxPrevPrev][k - 1] +
                                       rows[idxCurr][k];
                }
            }
        }

        /* Rotate indices for next iteration */
        idxPrevPrev = idxCurr;
        idxCurr = idxNext;
    }
}
