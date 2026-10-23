/*
 * A027865  Sums of six consecutive squares: a(n) = n^2 + (n+1)^2 + (n+2)^2 +
 *          (n+3)^2 + (n+4)^2 + (n+5)^2.
 * https://oeis.org/A027865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 30*n + 55
 * Range:     u8 n <= 3, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to seq_t before performing arithmetic
       to avoid unintended truncation or overflow on the index type.  */
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)6 * nn * nn + (seq_t)30 * nn + (seq_t)55;
    return term;
}
