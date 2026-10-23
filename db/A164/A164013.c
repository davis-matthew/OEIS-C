/*
 * A164013  3 times centered triangular numbers: a(n) = 9*n*(n+1)/2 + 3.
 * https://oeis.org/A164013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*n*(n+1)/2 + 3
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 1);  // n*(n+1)
    val /= 2;                   // divide by 2
    val *= 9;                   // multiply by 9
    val += 3;                   // add 3
    return val;
}
