/*
 * A130460  Infinite lower triangular matrix,(1,0,0,0,...) in the main
 *          diagonal and (1,2,3,...) in the subdiagonal.
 * https://oeis.org/A130460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r-1 if n = r(r-1)/2 + (r-1), else 0; where r =
 *            ceil((sqrt(1+8n)-1)/2). The sequence is the row‑by‑row
 *            concatenation of an infinite lower triangular matrix with main
 *            diagonal (1,0,0,…) and subdiagonal (1,2,3,…)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root for unsigned __int128 */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = x;
    while (lo < hi) {
        unsigned __int128 mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* sequence element */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety, offset is 1 */
    if (n == 1) return (seq_t)1;    /* a(1) = 1 */

    /* determine row r such that r(r+1)/2 >= n */
    unsigned __int128 calc = (unsigned __int128)1 + (unsigned __int128)8 * n;
    unsigned __int128 sqrt_val = isqrt128(calc);
    idx_t r = (idx_t)((sqrt_val - 1) / 2);

    /* correct possible under‑estimation of r */
    if ((unsigned __int128)r * (r + 1) / 2 < n)
        r++;

    /* previous cumulative length up to row r-1 */
    idx_t prev = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    if (n - prev == r - 1)          /* second‑to‑last element of row r */
        return (seq_t)(r - 1);

    return 0;                       /* all other positions are zero */
}
