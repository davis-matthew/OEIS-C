/*
 * A165162  Triangle T(n,m) with 2*n-1 entries per row, read by rows: the
 *          first n entries count down from n to 1, the remaining n-1 entries
 *          down from n-1 to 1.
 * https://oeis.org/A165162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = value of the triangle T(r,p) where r = ceil(sqrt(n)), p =
 *            n - (r-1)^2 (1‑based position inside the row), T(r,p) = r-p+1
 *            for 1 ≤ p ≤ r, = 2*r-p for r < p ≤ 2*r-1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) – the n‑th term of OEIS A165162. */
seq_t a(idx_t n)
{
    /* Find the smallest r with r*r >= n (i.e. r = ceil(sqrt(n))).
       Use binary search with 128‑bit multiplication to avoid overflow. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Compare mid*mid with n safely. */
        if (((__int128)mid * mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */

    /* Number of entries before this row: (r-1)^2 */
    idx_t prev_sq = (r == 1) ? 0 : (r - 1) * (r - 1);
    idx_t p = n - prev_sq;              /* position inside the row (1‑based) */

    /* Compute the triangle value. */
    if (p <= r)
        return (seq_t)(r - p + 1);      /* first part of the row   */
    else
        return (seq_t)(2 * r - p);      /* second part of the row  */
}
