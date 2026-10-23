/*
 * A024182  Integer part of ((4th elementary symmetric function of
 *          2,3,...,n+4)/(3rd elementary symmetric function of 2,3,...,n+4)).
 * https://oeis.org/A024182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+5)/8)
 * Range:     u8 n <= 42, u16 n <= 721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset starts at 1; behavior for n < 1 is irrelevant. */
    if (n < 1) return 0;
    seq_t nn = (seq_t)n;
    return (nn * (nn + 5)) / 8;
}
