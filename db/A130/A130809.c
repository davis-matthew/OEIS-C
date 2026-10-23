/*
 * A130809  If X_1, ..., X_n is a partition of a 2n-set X into 2-blocks then
 *          a(n) is equal to the number of 3-subsets of X containing none of
 *          X_i, (i=1,...,n).
 * https://oeis.org/A130809
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/3)*n*(n-1)*(n-2) for n >= 3
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* below the offset values are irrelevant */
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn - 1) * (nn - 2); /* n*(n-1)*(n-2) */
    val /= 3;              /* divide first to keep integer */
    val *= 4;              /* multiply by 4 */
    return val;
}
