/*
 * A125849  a(n) = Sum_{m=1..n-1} floor(m(n-2)/2)^2.
 * https://oeis.org/A125849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n-1} floor(m*(n-2)/2)^2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sum is empty or contains only zeros. */
    if (n < 2)
        return (seq_t)0;

    seq_t result = 0;
    seq_t n_minus_2 = (seq_t)n - (seq_t)2;

    for (idx_t m = 1; m < n; ++m) {
        seq_t t = (seq_t)m * n_minus_2 / (seq_t)2;   /* floor division */
        result += t * t;
    }
    return result;
}
