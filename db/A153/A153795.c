/*
 * A153795  5 times octagonal numbers: a(n) = 5*n*(3*n-2).
 * https://oeis.org/A153795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(3*n-2)
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn * (seq_t)3 - (seq_t)2);
    t *= (seq_t)5;
    return t;
}
