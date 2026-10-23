/*
 * A026041  a(n) = d(n)/2, where d = A026040.
 * https://oeis.org/A026041
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2-3*n+14)/6
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^2 and n^3 as seq_t to avoid overflow in intermediate steps */
    seq_t n2 = (seq_t)n * (seq_t)n;           /* n^2  */
    seq_t n3 = n2 * (seq_t)n;                 /* n^3  */
    /* val = n^3 - 3*n^2 + 14*n   (fits in seq_t) */
    seq_t val = n3 - (seq_t)3 * n2 + (seq_t)14 * (seq_t)n;
    /* final division by 6 (exact division for integer n >= 0) */
    return val / (seq_t)6;
}
