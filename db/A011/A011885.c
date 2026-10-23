/*
 * A011885  a(n) = floor(n*(n-1)/32).
 * https://oeis.org/A011885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/32)
 * Range:     u8 n <= 91, u16 n <= 1448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    /* compute n*(n-1)/32 exactly */
    seq_t nn = (seq_t)n;
    return (nn * (nn - 1u)) / 32u;
}
