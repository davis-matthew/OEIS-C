/*
 * A167487  a(n) = n*(n + 3)/2 + 8.
 * https://oeis.org/A167487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 3)/2 + 8
 * Range:     u8 n <= 14, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A167487: a(n) = n*(n+3)/2 + 8 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + (seq_t)3);
    res /= (seq_t)2;
    res += (seq_t)8;
    return res;
}
