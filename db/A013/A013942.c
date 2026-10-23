/*
 * A013942  Triangle of numbers T(n,k) = floor(2n/k), k=1..2n, read by rows.
 * https://oeis.org/A013942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   a(n) is the n-th element of the triangle T(i,j)=floor(2i/j),
 *            read row by row (i≥1, j=1..2i)
 * Range:     u8 n <= 9456, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t N)
{
    if (N == 0) return 0;          /* undefined for N=0, return 0 */

    /* Find the row r such that (r-1)*r < N ≤ r*(r+1) */
    idx_t lo = 1, hi = N;          /* hi can be N because r ≤ N */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* use 128‑bit to avoid overflow in mid*mid+mid */
        unsigned __int128 val = (unsigned __int128)mid * mid + mid;
        if (val >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;  /* row index */

    /* Compute column index k (1‑based) within the row */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r;
    idx_t k = N - (idx_t)prev_sum;   /* 1 ≤ k ≤ 2r */

    /* Compute floor(2r/k) using seq_t arithmetic */
    seq_t numerator = (seq_t)2 * (seq_t)r;
    seq_t result   = numerator / (seq_t)k;
    return result;
}
