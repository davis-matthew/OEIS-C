/*
 * A178067  Triangle read by rows: T(n,k) = (n^2 + k)*(n - k + 1)/2.
 * https://oeis.org/A178067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) = (r^2 + k)*(r - k + 1)/2, where r and k are the
 *            row and column of the n-th term in the lower triangular array
 *            read row-wise
 * Range:     u8 n <= 28, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t
find_row(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 >= n.  */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                /* r = low  */
}

seq_t
a(idx_t n)
{
    /* Handle the offset (a(1) is the first element).  */
    if (n == 0)
        return (seq_t)0;      /* Optional: behaviour below the offset. */

    /* Determine the row r and the column k for the given index n.  */
    idx_t r = find_row(n);
    idx_t prev_terms = (r - 1) * r / 2;   /* t_{r-1} */
    idx_t k = n - prev_terms;             /* 1 <= k <= r */

    /* Compute T(r,k) = (r^2 + k)*(r - k + 1)/2.  */
    seq_t part1 = (seq_t)r * r + (seq_t)k;        /* r^2 + k   */
    seq_t part2 = (seq_t)r - (seq_t)k + 1;        /* r - k + 1 */
    return part1 * part2 / 2;
}
