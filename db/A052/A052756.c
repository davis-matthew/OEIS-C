/*
 * A052756  E.g.f.: (-1/3)*LambertW(-3*x).
 * https://oeis.org/A052756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0, if n = 0; a(n) = (3*n)^(n-1) for n > 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t base = (seq_t)n * ((seq_t)3);
    seq_t result = 1;
    /* Multiply base (n-1) times */
    for (idx_t i = 1; i < n; ++i)
        result *= base;
    return result;
}
