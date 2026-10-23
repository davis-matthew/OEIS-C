/*
 * A151890  Triangle read by rows: T(l,c) = 2*l*c + l + c (0 <= c <= l).
 * https://oeis.org/A151890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) with r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(r,c) = 2*r*c + r + c
 * Range:     u8 n <= 76, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the greatest r such that r*(r+1)/2 <= n using binary search. */
    idx_t low  = 0;
    idx_t high = 1;
    while ((seq_t)high * (high + 1) / 2 <= n)
        high <<= 1;           /* exponential search for an upper bound */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }

    idx_t r = low;
    seq_t start = (seq_t)r * (r + 1) / 2;          /* index where row r starts   */
    seq_t c = (seq_t)n - start;                   /* column within the row     */
    seq_t result = (seq_t)2 * r * c + r + c;       /* T(r,c)                     */
    return result;
}
