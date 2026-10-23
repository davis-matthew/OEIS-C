/*
 * A156752  a(n) = floor(Catalan(n+1)/Catalan(n)).
 * https://oeis.org/A156752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(Catalan(n+1)/Catalan(n))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n < 4)
        return (seq_t)2;
    return (seq_t)3;
}
