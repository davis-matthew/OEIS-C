/*
 * A180118  a(n) = Sum_{k=1..n} (k+2)!/k! = Sum_{k=1..n} (k+2)*(k+1).
 * https://oeis.org/A180118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+6*n+11)/3
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t ns = (seq_t)n;
    seq_t num = ns * (ns * ns + 6 * ns + (seq_t)11);
    return num / (seq_t)3;
}
