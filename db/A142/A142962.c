/*
 * A142962  Scaled convolution of (n^3)*A000984(n) with A000984(n).
 * https://oeis.org/A142962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(3+5*n)/2
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 951, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;          /* n² */
    seq_t inner = (seq_t)3 + (seq_t)5 * (seq_t)n;  /* 3 + 5·n */
    seq_t prod = n2 * inner;                 /* n²·(3+5·n) */
    return prod / (seq_t)2;                  /* divide by 2 */
}
