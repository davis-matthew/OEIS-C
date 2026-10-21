/*
 * A094373  Expansion of (1-x-x^2)/((1-x)*(1-2*x)).
 * https://oeis.org/A094373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0 a(n) = 2^(n-1) + 1 for n > 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)1;

    /* compute 2^(n-1) by repeated doubling */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow = pow * (seq_t)2;

    return pow + (seq_t)1;
}
