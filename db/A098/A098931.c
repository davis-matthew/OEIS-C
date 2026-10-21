/*
 * A098931  a(0) = 1, a(n) = 1 + 2*3 + 4*5 + 6*7 + ... + (2n)*(2n+1) for n >
 *          0.
 * https://oeis.org/A098931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 1 + Σ_{k=1}^{n} (2k)(2k+1) = 1 + n(n+1)(4n+5)/3
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1377, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 1 + (nn * (nn + 1) * (4 * nn + 5)) / 3;
    return term;
}
