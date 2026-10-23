/*
 * A140869  Triangle read by rows where T(m,n) = floor((2mn+m+n-2)/2), m >= n
 *          >= 1.
 * https://oeis.org/A140869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = floor((2*m*n + m + n - 2)/2) with m =
 *            ceil((sqrt(8*n+1)-1)/2) n = n - (m-1)*m/2 (the column index in
 *            row m)
 * Range:     u8 n <= 135, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the triangle read by rows defined by T(m,n) = floor((2mn+m+n-2)/2). */
seq_t a(idx_t n)
{
    /* Find the row m such that m*(m+1)/2 >= n. */
    idx_t lo = 1, hi = 1;
    /* Expand hi until the condition holds (using 128‑bit to avoid overflow). */
    while (((__int128)hi * (hi + 1) / 2) < n) {
        hi <<= 1;
    }
    /* Binary search for the exact row. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((__int128)mid * (mid + 1) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;
    /* Compute the column index within row m. */
    idx_t start_before_m = (idx_t)((__int128)(m - 1) * m / 2);
    idx_t col = n - start_before_m;  /* 1-based */

    /* Compute T(m, col) using 128‑bit arithmetic to avoid overflow. */
    __int128 temp = (__int128)2 * (__int128)m * (__int128)col
                    + (__int128)m + (__int128)col - 2;
    return (seq_t)(temp / 2);
}
