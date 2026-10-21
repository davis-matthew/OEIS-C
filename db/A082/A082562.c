/*
 * A082562  a(n) = number of values of m such that m can be expressed as the
 *          sum of distinct odd numbers with largest odd number in the sum =
 *          2n+1.
 * https://oeis.org/A082562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2-1 for n>2; a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    seq_t nn = (seq_t)n;
    return nn * nn - 1;
}
