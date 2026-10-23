/*
 * A185878  Accumulation array of A185877, by antidiagonals.
 * https://oeis.org/A185878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col), where (row,col) are the coordinates of the
 *            n-th element in the array T(n,k) enumerated along antidiagonals.
 *            Specifically, T(n,k) = k*n*(2*k^2 -3*k +3*k*n -3*n +7)/6 for
 *            k>=1, n>=1. The antidiagonal numbering starts with diagonal 1
 *            containing T(1,1). Within each diagonal the ent...
 * Range:     u8 n <= 29, u16 n <= 536, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                   /* diagonal number (1-based) */
    __int128 prev128 = (__int128)(d - 1) * d / 2;
    idx_t prev = (idx_t)prev128;     /* elements before this diagonal */
    idx_t pos = n - prev;            /* position within the diagonal (1..d) */

    idx_t row = pos;
    idx_t col = d + 1 - pos;

    /* Compute T(row,col) */
    __int128 a_big = col;
    __int128 b_big = row;
    __int128 inner = 2*a_big*a_big - 3*a_big + 3*a_big*b_big - 3*b_big + 7;
    __int128 num = a_big * b_big * inner;
    __int128 res128 = num / 6;
    return (seq_t)res128;
}
