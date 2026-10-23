/*
 * A028895  5 times triangular numbers: a(n) = 5*n*(n+1)/2.
 * https://oeis.org/A028895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(n+1)/2
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 1);
    res /= 2;
    res *= 5;
    return res;
}
