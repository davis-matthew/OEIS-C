/*
 * A232495  a(n) = 9*n^3/2 - 21*n^2/2 + 8*n - 4.
 * https://oeis.org/A232495
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^3/2 - 21*n^2/2 + 8*n - 4
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 783, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast index to the arithmetic type once */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    seq_t val = 9 * n3 - 21 * n2 + 16 * nn - 8;
    val = val / 2;
    return val;
}
