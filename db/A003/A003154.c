/*
 * A003154  Centered 12-gonal numbers, or centered dodecagonal numbers:
 *          numbers of the form 6*k*(k-1) + 1.
 * https://oeis.org/A003154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + 6*n*(n-1)
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 1 + 6 * sn * (sn - 1);
}
