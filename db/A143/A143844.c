/*
 * A143844  Triangle T(n,k) = k^2 read by rows.
 * https://oeis.org/A143844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (n - r*(r+1)/2)^2, where r is the largest integer such
 *            that * r*(r+1)/2 <= n. Equivalently, r =
 *            floor((sqrt(8*n+1)-1)/2). This is the * square of the column
 *            index within the triangular array of squares
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r: largest r with r*(r+1)/2 <= n.  Use binary search. */
    idx_t lo = 0, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;                         /* increase hi until condition fails */

    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t r = lo;

    /* Compute column index k and its square. */
    unsigned __int128 tri_r = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - (idx_t)tri_r;
    seq_t k_sq = (seq_t)((unsigned __int128)k * k);
    return k_sq;
}
