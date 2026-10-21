/*
 * A051126  Table T(n,k) = n mod k read by downward antidiagonals (n >= 1, k
 *          >= 1).
 * https://oeis.org/A051126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = (r mod k) where r and k are determined from n by locating
 *            the antidiagonal and the row within that diagonal. Row r
 *            increases while k decreases so that r + k - 1 equals the
 *            diagonal number
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n.  Use a
       binary search with 128‑bit arithmetic to avoid overflow.  */
    idx_t lo = 1, hi = 1;
    while ( ((unsigned __int128)hi * (hi + 1)) / 2 < n )
    {
        /* Increase hi exponentially until it covers n.  */
        hi <<= 1;
        if (hi == 0)  /* overflow protection */
            hi = (idx_t)-1;
    }
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo; /* diagonal number (1‑based) */

    /* Number of elements before this diagonal:
       C(d-1) = (d-1)*d/2.  */
    idx_t prev = ((unsigned __int128)(d - 1) * d) / 2;

    /* Position within the diagonal (0‑based): */
    idx_t p = n - prev - 1;

    /* Row r is 1‑based within the diagonal: */
    idx_t r = p + 1;

    /* Column k decreases as the row increases: */
    idx_t k = d - r + 1;

    /* Compute r mod k.  Cast to seq_t for the result. */
    return (seq_t)r % (seq_t)k;
}
