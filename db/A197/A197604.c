/*
 * A197604  a(n) = floor((n+1/n)^5).
 * https://oeis.org/A197604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/n)^5)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n2 * n2;
    seq_t n5 = n3 * n2;
    seq_t res = n5 + 5 * n3 + 10 * nn;
    seq_t extra = (10 * n4 + 5 * n2 + 1) / n5;
    return res + extra;
}
