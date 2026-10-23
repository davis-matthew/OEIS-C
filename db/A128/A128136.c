/*
 * A128136  A128132 * A002260.
 * https://oeis.org/A128136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r-1)*c if c < r, else r^2; where r =
 *            ceil((sqrt(8*n+1)-1)/2) is the unique integer with r(r-1)/2 < n
 *            ≤ r(r+1)/2 and c = n - r(r-1)/2
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A128136 (row‑major enumeration of the
   triangle with entries (r-1)*c for c<r and r^2 for c=r).  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* guard – sequence is defined for n≥1 */

    /* binary search for row r: r(r+1)/2 ≥ n */
    idx_t low = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri =
            (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* previous triangular number tprev = (r-1)*r/2 */
    unsigned __int128 tprev =
        (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)tprev;   /* column index 1 … r */

    /* value in the triangle */
    if (c == r) {
        unsigned __int128 val = (unsigned __int128)r * r;
        return (seq_t)val;
    } else {
        unsigned __int128 val = (unsigned __int128)(r - 1) * c;
        return (seq_t)val;
    }
}
