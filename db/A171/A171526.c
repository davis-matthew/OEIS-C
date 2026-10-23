/*
 * A171526  Denominator of (n-th noncomposite/n).
 * https://oeis.org/A171526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 3, otherwise a(n) = n
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;
    return (seq_t)n;
}
