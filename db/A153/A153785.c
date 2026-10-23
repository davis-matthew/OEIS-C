/*
 * A153785  5 times heptagonal numbers: a(n) = 5*n*(5*n-3)/2.
 * https://oeis.org/A153785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(5*n-3)/2
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t five_n = (seq_t)n * 5;
    seq_t val = five_n * (five_n - 3);
    return val / 2;
}
