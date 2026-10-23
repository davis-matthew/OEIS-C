/*
 * A175724  Partial sums of floor(n^2/12).
 * https://oeis.org/A175724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((2*n^3 + 3*n^2 - 18*n)/72) Equivalent integer
 *            division: a(n) = (2*n^3 + 3*n^2 - 18*n + 36)/72 for all n ≥ 0
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1722, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                      /* cast index to seq_t */
    seq_t numerator = 2*nn*nn*nn + 3*nn*nn   /* 2·n³ + 3·n²   */
                    - 18*nn + 36;            /* -18·n + 36 for rounding   */
    return numerator / 72;                   /* integer division truncates   */
}
