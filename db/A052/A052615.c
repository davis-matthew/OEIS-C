/*
 * A052615  E.g.f. x^3*(1+2x-2x^2)/(1-x).
 * https://oeis.org/A052615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n < 3 a(3) = 6 a(4) = 72 a(n) = n! for n >= 5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small cases explicitly */
    if (n < 3)                return 0;
    if (n == 3)               return 6;
    if (n == 4)               return 72;

    /* For n >= 5 compute n! iteratively */
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;

    return result;
}
