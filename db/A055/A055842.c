/*
 * A055842  Expansion of (1-x)^2/(1-5*x).
 * https://oeis.org/A055842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1, 3, for n=0,1 respectively; for n>=2, a(n) = 16 *
 *            5^(n-2) (expansion of (1−x)^2/(1−5x))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)3;

    /* n >= 2 */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow5 *= (seq_t)5;
    return (seq_t)16 * pow5;
}
