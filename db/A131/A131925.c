/*
 * A131925  2*A002024 - A000012(signed).
 * https://oeis.org/A131925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*(r+1) - ((r+c)%2==0 ? 1 : -1) where (r,c) are the row
 *            and column of n in the triangular array with rows of length
 *            1,2,3, ... (row 0 starts at n=0)
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Use __int128 for safety against overflow. */
    idx_t r = 0;
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n)
        ++r;

    /* Column offset within the row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t col = n - row_start;

    /* 2*(r+1) as the base value */
    seq_t base = (seq_t)(r + 1) * (seq_t)2;

    /* If r+col is even, subtract 1; otherwise add 1. */
    if ( ((r + col) & 1) == 0 )
        return base - 1;
    else
        return base + 1;
}
