/*
 * A081551  Triangle, read by rows, in which the n-th row contains n smallest
 *          n-digit numbers.
 * https://oeis.org/A081551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = 10^(r-1)+(k-1), where r = ceil((sqrt(8*n+1)-1)/2), k = n
 *            - r*(r-1)/2
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 210, u128 n <= 780,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t sum = 1;          /* cumulative count up to current row */
    while (sum < n)
    {
        ++r;
        sum += r;
    }

    /* Number of elements before this row */
    idx_t prev_total = sum - r;
    idx_t k = n - prev_total;          /* 1‑based column within the row */

    /* Compute 10^(r-1) */
    seq_t power = 1;
    for (idx_t i = 1; i < r; ++i)
        power *= 10;

    return power + (k - 1);
}
