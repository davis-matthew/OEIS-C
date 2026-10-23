/*
 * A011930  a(n) = floor(n(n-1)(n-2)(n-3)/20).
 * https://oeis.org/A011930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/20)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 300, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 4 the value is 0. */
seq_t a(idx_t n)
{
    /* Return 0 immediately for the first three indices. */
    if (n < 4)
        return 0;

    /* Compute the quartic product using seq_t arithmetic. */
    seq_t result = (seq_t)n;
    result *= (n - 1);
    result *= (n - 2);
    result *= (n - 3);

    /* Integer division truncates toward zero, which is the same as floor
       for non-negative numerators. */
    result /= 20;

    return result;
}
