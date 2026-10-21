/*
 * A097810  a(n) = 7*2^n - 3*n - 6.
 * https://oeis.org/A097810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*2^n - 3*n - 6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n by repeated multiplication */
    seq_t pow2 = 1;
    const seq_t two = (seq_t)2;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= two;

    return (seq_t)7 * pow2 - (seq_t)3 * n - (seq_t)6;
}
