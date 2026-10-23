/*
 * A128622  Triangle T(n, k) = A128064(unsigned) * A128174, read by rows.
 * https://oeis.org/A128622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r - ((r + k) mod 2) where r = ceil((sqrt(8*n + 1) - 1) /
 *            2) -- the row number (1‑based) k = n - r*(r-1)/2 -- column
 *            number (1‑based)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n  */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1; /* (low+high)/2 */
        /* mid*(mid+1)/2 fits in idx_t for the required n */
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column k (1‑based) in that row */
    idx_t prev = ((r - 1) * r) / 2;   /* sum of first r-1 rows */
    idx_t k = n - prev;               /* 1 <= k <= r */

    /* Triangle entry T(r,k) = r - ((r + k) mod 2) */
    seq_t val = (seq_t)r;
    if ( ((r + k) & 1) != 0 )        /* subtract 1 if (r+k) is odd */
        val -= 1;

    return val;
}
