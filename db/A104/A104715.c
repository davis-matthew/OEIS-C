/*
 * A104715  Triangle T(n,k) = (2*k-1)*A000217(n-k+1) read by rows, 1<=k<=n.
 * https://oeis.org/A104715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = * (2*k-1) * ( (r-k+1)*(r-k+2)/2 ), * where r is the row
 *            such that r*(r+1)/2 ≥ n and * k = n - (r-1)*r/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* finds the smallest r with r*(r+1)/2 ≥ n  (n ≥ 1) */
static inline idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    /* grow high until it satisfies the condition */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* binary search between low+1 and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* out of range values are undefined */
    idx_t r = find_row(n);                      /* row number */
    idx_t prev = (idx_t)(((unsigned __int128)(r - 1) * r) / 2); /* cumulative count up to previous row */
    idx_t k = n - prev;                           /* column within the row (1‑based) */

    /* t = r - k + 1, the argument of the triangular number */
    idx_t t = r - k + 1;
    unsigned __int128 tri = (unsigned __int128)t * (t + 1) / 2;   /* (t*(t+1))/2  */
    unsigned __int128 mult = (unsigned __int128)(2 * k - 1);      /* (2k-1)      */
    unsigned __int128 val = tri * mult;                          /* full product */

    return (seq_t)val;   /* cast to the built‑in sequence type   */
}
