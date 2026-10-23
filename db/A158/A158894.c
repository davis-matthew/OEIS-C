/*
 * A158894  Sawtooth pattern of one, then two, then three, then four etc.
 *          consecutive odd numbers, starting each time at 3.
 * https://oeis.org/A158894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 2*(pos-1) where * pos = n - k*(k-1)/2 and k =
 *            smallest integer such that k*(k+1)/2 >= n. * In other words, the
 *            sequence consists of blocks of odd numbers * 3,5,7,... of
 *            increasing length 1,2,3,4,
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   triangular_lt_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_lt_n.h"

/* a(n) : 1‑based index */
seq_t a(idx_t n)
{
    /* Find k such that k*(k+1)/2 >= n .
       Binary search on [1, n]. */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular_lt_n(mid, n))
            low = mid + 1;
        else
            high = mid;
    }
    idx_t k = low;

    /* T_{k-1} = (k-1)*k/2  (fits in idx_t because it is < n) */
    idx_t t_kminus1 = ( ( (__int128)(k - 1) * k ) / 2 );

    /* Position within the block (1‑based) */
    idx_t pos = n - t_kminus1;   /* 1 ≤ pos ≤ k */

    /* a(n) = 3 + 2*(pos-1) */
    return (seq_t)3 + (seq_t)2 * ( (seq_t)(pos - 1) );
}
