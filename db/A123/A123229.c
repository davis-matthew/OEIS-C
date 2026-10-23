/*
 * A123229  Triangle read by rows: T(n, m) = n - (n mod m).
 * https://oeis.org/A123229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, m) where r is the row number such that r*(r+1)/2 >=
 *            n, and m = n - r*(r-1)/2. T(r, m) = m * floor(r/m) = r - (r mod
 *            m)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the defined range */

    idx_t r = 1;
    idx_t cumulative = 1;          /* sum of rows up to current r */

    /* Find the smallest r with cumulative >= n */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    idx_t prev_cumulative = cumulative - r;   /* sum up to row r-1 */
    idx_t m = n - prev_cumulative;           /* column within the row */

    /* Compute T(r, m) = m * floor(r/m) */
    seq_t val = (seq_t)m * (seq_t)(r / m);
    return val;
}
