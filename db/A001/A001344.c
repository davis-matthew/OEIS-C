/*
 * A001344  a(n) = Sum_{k=0..2} (n+k)! * C(2,k).
 * https://oeis.org/A001344
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * ((n+1)^2 + 3*(n+1) + 1) = n! * (n^2 + 5*n + 5) for n
 *            >= 0, a(-1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    seq_t m;
    idx_t i;
    if (n < 0) return (seq_t)2;
    for (i = 2; i <= n; i++) fact *= (seq_t)i;
    m = (seq_t)n;
    return fact * (m * (m + 5) + 5);
}
