/*
 * A157528  Triangle read by rows: T(n, k) = 2*k*(n - k) with T(n, 0) = T(n,
 *          n) = 1.
 * https://oeis.org/A157528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 for the first or last element of a row; otherwise a(n)
 *            = 2*k*(r-k) where r is the row index: r =
 *            floor((sqrt(8*n+1)-1)/2) k = n - r*(r+1)/2 (column index within
 *            the row)
 * Range:     u8 n <= 285, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binary search for the largest r such that r*(r+1)/2 <= n */
    idx_t low = 0;
    idx_t high = n;          /* high is inclusive */

    while (low < high) {
        /* mid biased up to keep loop moving towards the upper bound */
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* start index of row r */
    __uint128_t start_r = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start_r;

    if (k == 0 || k == r)
        return (seq_t)1;

    /* compute 2*k*(r-k) in a wider type to avoid overflow */
    __uint128_t term = (__uint128_t)2 * k * (r - k);
    return (seq_t)term;
}
