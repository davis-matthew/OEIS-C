/*
 * A143979  Rectangular array R by antidiagonals: label each unit square in
 *          the first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares having |x-y| = 0 (mod 3); then R(m,n) is the number of
 *          UNmarked squares in the rectangle [0,m] X [0,n].
 * https://oeis.org/A143979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (m+1)*(n+1) - ceil(((m+1)*(n+1))/3) where (m,n) are the
 *            coordinates of the n‑th term in the array read along
 *            antidiagonals starting at (0,0). Specifically, let d =
 *            ⌈(⌈√(1+8n)⌉−2)/2⌉, prev = d(d+1)/2, i = n−prev−1, then m = d−i
 *            and n = i
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square root (floor) of a 128‑bit value */
static inline idx_t int_sqrt128(__int128 n)
{
    long double d = sqrt((long double)n);
    uint64_t x = (uint64_t)d;
    /* Correct possible rounding errors */
    while (( (__int128)(x + 1) * (x + 1) ) <= n) ++x;
    while (( (__int128)x * x ) > n) --x;
    return (idx_t)x;
}

seq_t a(idx_t n)
{
    /* Compute ceil(sqrt(1+8n))  */
    __int128 N = (__int128)1 + (__int128)8 * n;
    idx_t sqrt_floor = int_sqrt128(N);
    if (( (__int128)sqrt_floor * sqrt_floor ) < N) ++sqrt_floor;   /* ceil */

    /* d = (ceil_sqrt - 2) / 2  */
    idx_t d = (sqrt_floor - (idx_t)2) / (idx_t)2;

    /* prev = d*(d+1)/2  */
    idx_t prev = (idx_t)((__int128)d * (d + 1) / (idx_t)2);

    /* position in diagonal */
    idx_t i = n - prev - 1;            /* 0‑based index within diagonal */
    idx_t m_coord = d - i;
    idx_t n_coord = i;

    /* compute R(m+1,n+1) = (m+1)*(n+1) - ceil(((m+1)*(n+1))/3) */
    __int128 prod128 = (__int128)(m_coord + 1) * (n_coord + 1);
    seq_t prod = (seq_t)prod128;
    seq_t ceil_div = (prod + (seq_t)2) / (seq_t)3;
    return prod - ceil_div;
}
