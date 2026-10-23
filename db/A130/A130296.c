/*
 * A130296  Triangle read by rows: T[i,1]=i, T[i,j]=1 for 1 < j <= i =
 *          1,2,3,...
 * https://oeis.org/A130296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = i if n = (i-1)*i/2 + 1 (the first element of row i),
 *            otherwise a(n) = 1. This computes the triangular‐row sequence
 *            A130296
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of range, but safe */

    idx_t sum = 0;   /* cumulative length of rows processed so far */
    for (idx_t row = 1; ; ++row) {
        sum += row;          /* now sum = T(row) */
        if (n <= sum) {      /* n lies inside or at the end of this row */
            if (n == sum - row + 1)   /* first element of row `row` */
                return (seq_t)row;
            else
                return 1;               /* all other elements are 1 */
        }
    }
}
