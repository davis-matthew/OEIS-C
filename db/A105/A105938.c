/*
 * A105938  a(n) = binomial(n+2,2)*binomial(n+5,2).
 * https://oeis.org/A105938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2) * binomial(n+5,2)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 359, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t t1 = (n0 + 1) * (n0 + 2) / 2;
    seq_t t2 = (n0 + 4) * (n0 + 5) / 2;
    return t1 * t2;
}
