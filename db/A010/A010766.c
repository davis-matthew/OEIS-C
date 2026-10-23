/*
 * A010766  Triangle read by rows: row n gives the numbers floor(n/k), k =
 *          1..n.
 * https://oeis.org/A010766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = value at position n when the infinite triangle
   T(n,k) = floor(n/k) (k = 1..n) is read row by row.
   For a given n, let r be the smallest integer with r(r+1)/2 ≥ n;
   let c = n - (r-1)r/2 (1‑based column). 
   Then a(n) = floor(r/c) = r / c.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* behaviour below offset is irrelevant; return 0 */
        return 0;

    /* Find the row r such that r(r+1)/2 ≥ n using binary search. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute the column index c. */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_sum;   /* 1‑based column */

    /* The desired term is the integer division r / c. */
    return (seq_t)(r / c);
}
