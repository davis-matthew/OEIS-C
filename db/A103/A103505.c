/*
 * A103505  Denominator in expansion of (1-x)*log(1-x).
 * https://oeis.org/A103505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = n*(n-1) for n ≥ 2; a(0) = a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;
    seq_t nn = n;
    return nn * (nn - 1);
}
