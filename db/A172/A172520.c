/*
 * A172520  Triangle in which row n gives the number of divisors of numbers in
 *          the range n to n+k for k=0..n-1.
 * https://oeis.org/A172520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of distinct positive divisors of all integers in
 *            the range row .. row+col, where (row, col) is the triangular
 *            coordinate of the linear index n in the triangle
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0, but safe */

    /* Find the row r such that T_{r-1} < n ≤ T_r, where T_r = r*(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;          /* number of terms before row r */
    idx_t k    = n - prev - 1;             /* 0‑based column index within row r */
    idx_t m    = r;                        /* starting integer of the range */
    idx_t end  = m + k;                    /* last integer of the range */

    seq_t cnt = 0;
    for (idx_t d = 1; d <= end; ++d) {
        idx_t low  = (m + d - 1) / d;      /* ceil(m/d) */
        idx_t high = end / d;              /* floor(end/d) */
        if (low <= high)
            ++cnt;
    }
    return cnt;
}
