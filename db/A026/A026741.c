/*
 * A026741  a(n) = n if n odd, n/2 if n even.
 * https://oeis.org/A026741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac, mult, changed
 * Formula:   a(n) = n if n odd, n/2 if n even
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)(n >> 1);
    else
        return (seq_t)n;
}
