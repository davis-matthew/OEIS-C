/*
 * A084624  a(n) = floor(C(n+5,5)/C(n+2,2)).
 * https://oeis.org/A084624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+3)*(n+4)*(n+5)/60)
 * Range:     u8 n <= 20, u16 n <= 153, u32 n <= 1948, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    return (n0 + (seq_t)3) * (n0 + (seq_t)4) * (n0 + (seq_t)5) / (seq_t)60;
}
