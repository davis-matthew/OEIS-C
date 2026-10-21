/*
 * A213553  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          (n-1+h)^3, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 120, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = T(r,c) where the array T is read by falling 
 *  antidiagonals of the rectangular convolution of b(h)=h and 
 *  c(h)=(r-1+h)^3.  T(r,c) = c * ( (3*c^4-5*c^2+2) 
 *           + 15*c*(c^2-1)*r + 30*(c^2-1)*r^2 + 30*(c+1)*r^3 ) / 60
 *  The index n is the position in this antidiagonal reading, with a(1)
 *  corresponding to T(1,1).  The algorithm finds the diagonal s such
 *  that s*(s+1)/2 ≥ n, then determines the row r and column c,
 *  evaluates the closed‑form expression, and returns the result.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* n<1 not used, but safe */

    /* Find diagonal s: smallest integer with s*(s+1)/2 >= n */
    idx_t lo = 1, hi = 2;
    /* Grow hi until it covers n */
    while ((unsigned __int128)hi * (hi + 1) / 2 < (unsigned __int128)n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= (unsigned __int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;                       /* current diagonal */
    idx_t cum_before = (s - 1) * s / 2;  /* entries before diagonal s */
    idx_t offset = n - cum_before - 1;  /* zero‑based index within diagonal */
    idx_t r = offset + 1;               /* row index */
    idx_t c = s - offset;               /* column index */

    /* Compute T(r,c) with 128‑bit intermediate arithmetic */
    unsigned __int128 cc = c;
    unsigned __int128 rr = r;
    unsigned __int128 term1 = 3 * cc * cc * cc * cc - 5 * cc * cc + 2;
    unsigned __int128 term2 = 15 * cc * (cc * cc - 1) * rr;
    unsigned __int128 term3 = 30 * (cc * cc - 1) * rr * rr;
    unsigned __int128 term4 = 30 * (cc + 1) * rr * rr * rr;
    unsigned __int128 num   = cc * (term1 + term2 + term3 + term4);
    seq_t result = (seq_t)(num / 60ull);
    return result;
}
