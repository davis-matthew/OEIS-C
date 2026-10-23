/*
 * A161984  Table read by rows: the number n followed by the odd numbers from
 *          n(n-1)+1 to (n-1)(n+2)+1.
 * https://oeis.org/A161984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is given by the table row r, where r =
 *            floor((sqrt(8*n+1)-1)/2). The index within the row is pos = n -
 *            r*(r+1)/2. If pos = 0, a(n) = r; otherwise a(n) = r*(r-1)+1 +
 *            2*(pos-1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t sum = 0;
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while (sum + r + 1 <= n) {
        sum += r + 1;
        r++;
    }
    idx_t pos = n - sum; /* 0 <= pos <= r */
    if (pos == 0)
        return (seq_t)r;
    /* start = r*(r-1)+1, then add 2*(pos-1) */
    seq_t start = (seq_t)r * (seq_t)(r - 1) + 1;
    return start + 2 * (seq_t)(pos - 1);
}
