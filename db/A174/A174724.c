/*
 * A174724  Sum_{k=1..n} Floor((k + 3*sqrt(k))/k).
 * https://oeis.org/A174724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n=1; 2*n+3 for 2<=n<=9; n+12 for n>=10
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)4;
    if (n <= 9)
        return (seq_t)n * (seq_t)2 + (seq_t)3;
    return (seq_t)n + (seq_t)12;
}
