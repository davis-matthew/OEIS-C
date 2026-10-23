/*
 * A151675  Row sums of A154685.
 * https://oeis.org/A151675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 9)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* not used for offset 1 but safe */
        return 0;
    seq_t nn = (seq_t) n;
    seq_t t   = ((seq_t)2 * nn * nn) + 5 * nn + 9;
    seq_t res = (nn * t) / 2;
    return res;
}
