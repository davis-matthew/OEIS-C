/*
 * A057028  Triangle T read by rows: row n consists of the numbers C(n,2)+1 to
 *          C(n+1,2); numbers in odd-numbered places form a decreasing
 *          sequence and the others an increasing sequence.
 * https://oeis.org/A057028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A057028  
    Triangle T read by rows: row n consists of the numbers C(n,2)+1 … C(n+1,2).  
    Within each row the elements are arranged so that moving from the left
    produces the indices  
          mid+1, mid, mid+2, mid‑1, mid+3, mid‑2, …  
    where mid = (n-1)/2.  This yields the required alternating decreasing
    (odd positions) / increasing (even positions) pattern.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 >= n   (1‑based) */
    idx_t lo = 1, hi = 1;
    while ( ((__int128)hi*(hi+1))/2 < n )
        hi <<= 1;            /* grow high enough */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;       /* middle point */
        if ( ((__int128)mid*(mid+1))/2 >= n )
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                  /* row number */

    /* position within the row (0‑based) */
    idx_t prev_tri = ( (__int128)(r-1)*r ) / 2;   /* T(r-1) */
    idx_t j        = n - prev_tri - 1;            /* 0‑based index */

    /* compute the index into the sorted list of numbers [C(r,2)+1 … C(r+1,2)] */
    idx_t mid_idx = (r-1) / 2;        /* mid = (r-1)/2 */

    int64_t offset;
    if (r & 1) {                    /* r is odd */
        if ((j & 1) == 0)           /* j even */
            offset = -(int64_t)(j / 2);
        else                        /* j odd  */
            offset = (int64_t)(j + 1) / 2;
    } else {                        /* r is even */
        if ((j & 1) == 0)           /* j even */
            offset = (int64_t)(j / 2) + 1;
        else                        /* j odd  */
            offset = -(int64_t)(j / 2);
    }

    idx_t idx = (idx_t)((int64_t)mid_idx + offset);   /* 0‑based in the row */

    /* base value of the row (smallest element) */
    seq_t base = ((seq_t)r*(r-1))/2 + 1;  /* C(r,2)+1 */

    return base + (seq_t)idx;
}
