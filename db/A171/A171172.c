/*
 * A171172  Triangle read by rows in which row n lists 3n-2 together with the
 *          first 2n-1 positive integers.
 * https://oeis.org/A171172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = 3r-2 if n==(r-1)r+1, otherwise a(n)=n-(r-1)r-1, where r
 *            is the smallest integer with r(r+1) >= n (the row index)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1) >= n */
    idx_t low  = 1;
    idx_t high = 1;
    /* Expand high until it is large enough */
    while ((unsigned __int128)high * (high + 1) < n) {
        high <<= 1;
    }
    /* Binary search for the minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* Compute the position inside the row (0‑based) */
    idx_t prev = (r - 1) * r;            /* total terms before row r */
    idx_t pos  = n - prev - 1;           /* 0 == first element of the row */

    if (pos == 0) {
        return (seq_t)3 * r - 2;         /* the special value 3r-2 */
    } else {
        return (seq_t)pos;               /* numbers 1 .. 2r-1 */
    }
}
