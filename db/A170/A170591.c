/*
 * A170591  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0, otherwise a(n) = 6 * 5^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first term directly */
    if (n == 0)
        return (seq_t)1;

    /* Compute 5^(n-1) iteratively to avoid external dependencies */
    seq_t pow5 = (seq_t)1;
    const seq_t five = (seq_t)5;
    for (idx_t i = 1; i < n; ++i) {
        pow5 *= five;
    }

    /* Multiply by 6 to get the final term */
    const seq_t six = (seq_t)6;
    return six * pow5;
}
