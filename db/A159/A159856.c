/*
 * A159856  Triangle read by rows: T(n,0) = n+1, T(n,k) = 2*T(n-1,k) -
 *          T(n-1,k-1), T(n,k) = 0 if k > n and if k < 0.
 * https://oeis.org/A159856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows.
   a(n) is the nth element of the array obtained by writing the triangle
   T(n,k) row‑by‑row.  The triangle satisfies
          T(n,0) = n+1
          T(n,k) = 2*T(n‑1,k) – T(n‑1,k‑1)   (k>0)
   and T(n,k)=0 for k>n or k<0.  The mapping from a(n) to (n,k) is
   n = T₀ + k where T₀ = r*(r+1)/2 and 0≤k≤r. */

/* Use the provided allocator for seq_t arrays. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ( (r * (r + 1)) / 2 <= n )
        r++;
    r--;                      /* last r with triangular number <= n */

    idx_t tri = (r * (r + 1)) / 2;   /* triangular number of row r */
    idx_t c   = n - tri;            /* column index within that row */

    /* Allocate two rows of length r+1; use the utilities so that
       constructors / destructors (if any) are handled correctly. */
    seq_t *prev = alloc_seq_array(r + 1);
    seq_t *cur  = alloc_seq_array(r + 1);

    /* Row 0: T(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r */
    for (idx_t i = 1; i <= r; ++i)
    {
        cur[0] = (seq_t)(i + 1);          /* T(i,0) */
        for (idx_t j = 1; j <= i; ++j)
        {
            /* T(i,j) = 2*T(i-1,j) - T(i-1,j-1);
               For j==i, T(i-1,i)=0, i.e. previous row element is missing. */
            seq_t a_val = (j < i) ? prev[j] : (seq_t)0;
            seq_t b_val = prev[j - 1];
            cur[j] = 2 * a_val - b_val;
        }
        /* Swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* Now prev holds row r */
    seq_t result = prev[c];

    /* Clean up */
    free_seq_array(prev);
    free_seq_array(cur);

    return result;
}
