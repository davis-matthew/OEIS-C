/*
 * A126372  Number of base 18 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑18 n‑digit strings (leading zeros allowed)
 *            whose adjacent digits differ by at most one. This is sequence
 *            A126372 with offset 0 (a(0)=1). The following dynamic
 *            programming algorithm computes a(n) exactly for all n and for
 *            all integer sizes supported by seq_t. It runs in O(n·1...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (empty string) */
    if (n == 0)
        return (seq_t)1;

    /* Arrays to hold counts for the previous and current length */
    seq_t dp_prev[18];
    seq_t dp_cur[18];
    int d;

    /* Initial row for length 1: all 18 digits are allowed, including 0 */
    for (d = 0; d < 18; ++d)
        dp_prev[d] = (seq_t)1;

    /* Build rows up to the desired length n.
       For k = 2..n we extend each string by one digit
       that differs by at most one from the last digit. */
    for (idx_t k = 2; k <= n; ++k) {
        for (d = 0; d < 18; ++d) {
            seq_t sum = dp_prev[d];          /* keep the same digit */
            if (d > 0)
                sum += dp_prev[d - 1];       /* append d-1 */
            if (d < 17)
                sum += dp_prev[d + 1];       /* append d+1 */
            dp_cur[d] = sum;
        }
        /* Copy current row to previous for next iteration */
        for (d = 0; d < 18; ++d)
            dp_prev[d] = dp_cur[d];
    }

    /* Sum the counts for all possible ending digits (0..17). */
    seq_t total = 0;
    for (d = 0; d < 18; ++d)
        total += dp_prev[d];

    return total;
}
