/*
 * A086078  Number of 7's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of occurrences of digit 7 in the decimal
 *            representation * of the triangular number n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 1187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t cnt = 0;
    while (tri > 0) {
        if ((tri % 10) == 7) cnt++;
        tri /= 10;
    }
    return cnt;
}
