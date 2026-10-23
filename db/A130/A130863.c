/*
 * A130863  Ratio of quadruple Sum of k^2-1 to quadruple sum of k made into an
 *          integer sequence: (1/6)*(-1 + n)*(2 + n)*(3 + n)*(7 + n).
 * https://oeis.org/A130863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+2)*(n+3)*(n+7)/6 (OEIS A130863)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety, treat n=0 as 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert index to the sequence type to avoid truncation. */
    seq_t nn = (seq_t)n;

    /* Compute each factor separately. */
    seq_t f1 = nn - 1;          /* n-1   */
    seq_t f2 = nn + 2;          /* n+2   */
    seq_t f3 = nn + 3;          /* n+3   */
    seq_t f4 = nn + 7;          /* n+7   */

    /* Multiply in a way that reduces overflow risk. */
    seq_t prod = f1 * f2;
    prod = prod * f3;
    prod = prod * f4;

    /* Divide by 6. Since 6 divides the product for all n, this is exact. */
    return prod / 6;
}
