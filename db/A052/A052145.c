/*
 * A052145  a(n) = (2n-1)*(2n-1)!/n.
 * https://oeis.org/A052145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-1)*(2n-1)!/n Equivalent: let m = 2n-1; a(n) = m * m!
 *            / n (with integer division). For n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; return 0 for safety. */
    if (n == 0)
        return 0;

    /* m = 2n-1, the odd integer */
    idx_t m = 2*n - 1;

    /* Compute m! iteratively. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;          /* use seq_t multiplication */

    /* Cancel the division by n before the final multiplication by m
       to keep intermediate values as small as possible. */
    fact /= n;
    fact *= (seq_t)m;

    return fact;
}
