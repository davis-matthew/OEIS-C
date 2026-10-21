/*
 * A095888  Triangle read by rows: T(n,k) = n^(n-k), n>=1, 1<=k<=n.
 * https://oeis.org/A095888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r^(r - k), where r = minimal integer with r*(r+1)/2 ≥ n,
 *            k = n - r*(r-1)/2 (the position of n in row r)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row r that contains the n-th element */
    idx_t r = 1;
    /* cumulative count up to current row: r*(r+1)/2 */
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* previous cumulative count up to row r-1 */
    idx_t prev = (r - 1) * r / 2;
    /* position within the row (1‑based) */
    idx_t k = n - prev;
    /* exponent: n - k = r - (k) */
    idx_t e = r - k;

    /* compute r^e */
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result *= (seq_t)r;

    return result;
}
