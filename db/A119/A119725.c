/*
 * A119725  Triangular array read by rows: T(n,1) = T(n,n) = 1, T(n,k) =
 *          3*T(n-1,k-1) + 2*T(n-1,k).
 * https://oeis.org/A119725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 144, u64 n <= 480,
 *            u128 n <= 1741, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular array T(n,k) read by rows:
   T(1,1) = 1,   T(n,1) = T(n,n) = 1,
   T(n,k) = 3*T(n-1,k-1) + 2*T(n-1,k)  for 1<k<n.
   a(n) is the n‑th term (row‑by‑row).
   Initial terms: a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=5, … */

/*   The code dynamically builds the triangle only up to the row
    containing the requested index.  Two rows are kept at a time
    (previous and current).  For C++ where seq_t might be an
    arbitrary‑precision class its constructors are invoked via
    new[].  For plain C we fall back to malloc.  This keeps the
    implementation both portable and safe.   */

#include <stdlib.h>

#if defined(__cplusplus)
    #define MEM_NEW(n)   new seq_t[n]
    #define MEM_DELETE(p) delete[] p
#else
    #define MEM_NEW(n)   ((seq_t*)malloc((n) * sizeof(seq_t)))
    #define MEM_DELETE(p) free(p)
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, unused */

    /* 1. Locate the row r that contains the n‑th element.   */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;

    /* 2. Allocate two rows of length r.  At most r elements
         are ever needed in any row.                                   */
    seq_t *prev = MEM_NEW(r);
    seq_t *cur  = MEM_NEW(r);
    if (!prev || !cur) {                   /* allocation failure   */
        MEM_DELETE(prev);
        MEM_DELETE(cur);
        return 0;
    }

    /* 3. Initialise first row (row 1). */
    prev[0] = 1;
    idx_t global_idx = 1;                  /* nth element seen so far */

    if (global_idx == n) {                 /* n==1                         */
        seq_t result = prev[0];
        MEM_DELETE(prev);
        MEM_DELETE(cur);
        return result;
    }

    /* 4. Build rows 2 .. r. */
    for (idx_t row = 2; row <= r; ++row)
    {
        /* first element of the new row */
        cur[0] = 1;
        ++global_idx;
        if (global_idx == n) {
            seq_t result = cur[0];
            MEM_DELETE(prev);
            MEM_DELETE(cur);
            return result;
        }

        /* interior elements */
        for (idx_t k = 1; k <= row - 2; ++k)
        {
            cur[k] = 3 * prev[k - 1] + 2 * prev[k];
            ++global_idx;
            if (global_idx == n) {
                seq_t result = cur[k];
                MEM_DELETE(prev);
                MEM_DELETE(cur);
                return result;
            }
        }

        /* last element of the row */
        cur[row - 1] = 1;
        ++global_idx;
        if (global_idx == n) {
            seq_t result = cur[row - 1];
            MEM_DELETE(prev);
            MEM_DELETE(cur);
            return result;
        }

        /* Prepare for next iteration: swap buffers */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* Should never reach here for valid n */
    MEM_DELETE(prev);
    MEM_DELETE(cur);
    return 0;
}
