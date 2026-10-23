/*
 * A130810  If X_1,...,X_n is a partition of a 2n-set X into 2-blocks then
 *          a(n) is equal to the number of 4-subsets of X containing none of
 *          X_i, (i=1,...,n).
 * https://oeis.org/A130810
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*binomial(n,4)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is undefined; return 0 */
    if (n < 4)
        return 0;

    seq_t prod = (seq_t)n * (n - 1) * (n - 2) * (n - 3);
    prod /= 24;      /* binomial(n,4) */
    prod *= 16;      /* 16 * C(n,4) */
    return prod;
}
