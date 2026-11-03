/*
 * A064578  Inverse permutation to A057027.
 * https://oeis.org/A064578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((r-1)*r)/2 + min(2*k-1, 2*(r-k+1)) where r = smallest
 *            integer with r(r+1)/2 >= n (block number), k = n - (r-1)*r/2
 *            (position within the block)
 * Range:     u8 n <= 254, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 88 terms through n = 9143 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the block number r for index n.
   r is the minimal integer such that r*(r+1)/2 >= n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Expand high until the triangular number is >= n */
    while ( ( (__int128)high * (high + 1) ) / 2 < n )
    {
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low + 1 < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid;
    }

    return high;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);                      /* block number */
    /* total elements before this block */
    idx_t prev_total = (idx_t)( ( (__int128)(r - 1) * r ) / 2 );
    idx_t k = n - prev_total;                   /* position within the block (1‑based) */

    seq_t base = ( (seq_t)(r - 1) * (seq_t)r ) / 2;      /* ((r-1)*r)/2 */
    seq_t t1   = ( (seq_t)2 * (seq_t)k ) - 1;             /* 2*k-1   */
    seq_t t2   = ( (seq_t)2 * (seq_t)(r - k + 1) );       /* 2*(r-k+1) */
    seq_t minv = t1 < t2 ? t1 : t2;

    return base + minv;
}
