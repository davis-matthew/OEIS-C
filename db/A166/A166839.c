/*
 * A166839  Number of nX2 1..5 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^5 + 35*n^4 + 523*n^3 + 3361*n^2 + 4396*n
 *            - 14436) / 5040
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 12, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;
    seq_t n2 = nn - 2;
    seq_t term = nn * n1 * n2;           /* n*(n-1)*(n-2) */

    seq_t n2sq = nn * nn;                /* n^2 */
    seq_t n3   = n2sq * nn;              /* n^3 */
    seq_t n4   = n3 * nn;                /* n^4 */
    seq_t n5   = n4 * nn;                /* n^5 */

    seq_t poly = n5;
    poly += 35 * n4;
    poly += 523 * n3;
    poly += 3361 * n2sq;
    poly += 4396 * nn;
    poly -= 14436;

    seq_t num = term * poly;             /* numerator */
    return num / 5040;                   /* divide by 7! */
}
