/*
 * A047089  Array T read by antidiagonals: T(h,k)=number of paths consisting
 *          of steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and touches the line y=x/2 only at lattice
 *          points.
 * https://oeis.org/A047089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = T(h,k)   where T(h,k) counts paths from (0,0) to (h,k) 
   using only up/right unit steps that may touch the line y = x/2 only at lattice
   points.  The coordinates (h,k) are obtained by reading the array ru-reading  
   by antidiagonals: for each n let m satisfy m*(m+1)/2 ≤ n < (m+1)*(m+2)/2,  
   offset = n - m*(m+1)/2, then h = offset (horizontal steps), k = m - offset. 
   The dynamic‑programming recurrence is:  

   T(0,*) = T(*,0) = 1
   T(x,y) = T(x-1,y) + (x == 2*y-1 ? 0 : T(x,y-1))  for x>0, y>0. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for OEIS sequence A047089 */
seq_t a(idx_t n)
{
    /* Find diagonal m containing index n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;   /* m*(m+1)/2 */
        if (val <= n) low = mid;
        else          high = mid - 1;
    }
    idx_t m = low;

    idx_t start   = (idx_t)((__int128)m * (m + 1) / 2);
    idx_t offset  = n - start;
    idx_t h = offset;          /* horizontal steps */
    idx_t k = m - offset;      /* vertical steps   */

    /* Allocate DP columns */
    if (k > sizeof(seq_t) * 8) { /* defensive */
        return (seq_t)0;
    }
    seq_t *prev = (seq_t *)calloc((size_t)k + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)k + 1, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base: column x = 0, all entries = 1 */
    for (idx_t y = 0; y <= k; ++y)
        prev[y] = (seq_t)1;

    /* DP over horizontal steps */
    for (idx_t x = 1; x <= h; ++x) {
        curr[0] = prev[0];                 /* T(x,0) = T(x-1,0) */
        for (idx_t y = 1; y <= k; ++y) {
            seq_t left = prev[y];           /* from (x-1,y) */
            seq_t up   = (x == ( (idx_t)2 * y - 1)) ? (seq_t)0 : curr[y - 1];
            curr[y] = left + up;
        }
        /* swap roles */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
