/*
 * A136261  Triangle T(n,k) = k*A122188(n,k), read by rows.
 * https://oeis.org/A136261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, sign
 * Formula:   a(n) = T(r,k) where T(r,k)=k*A122188(r,k) with A122188(r,k) =
 *            ±1: if (r is odd) first r‑1 entries are +1, last is –1; if (r is
 *            even) first r‑1 entries are –1, last is +1
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term is computed as a signed multiple of its column index k. */

seq_t a(idx_t n)
{
    /* Find row r such that  r*(r+1)/2 >= n and (r-1)*r/2 < n. */
    idx_t low = 1;
    idx_t high = n;          /* upper bound: n is safe for binary search  */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row number (1‑based)  */
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - before;             /* column number, 1‑based  */

    seq_t sign = ((r & 1) ? 1 : -1);   /* base sign for columns 1..r-1  */
    if (k == r)                        /* last column has opposite sign */
        sign = -sign;

    return sign * (seq_t)k;
}
