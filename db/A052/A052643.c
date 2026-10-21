/*
 * A052643  E.g.f. (1+x-x^2)^2/(1-x)^2.
 * https://oeis.org/A052643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3) * n! for n >= 3, with a(0) = 1, a(1) = 4, a(2) = 12
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special small cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;

    seq_t fact = (seq_t)1;
    /* compute n! as product 1*2*…*n */
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* multiply by (n+3) and return */
    return fact * (seq_t)(n + 3);
}
