/*
 * A028884  a(n) = (n + 3)^2 - 8.
 * https://oeis.org/A028884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)^2 - 8
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;     /* convert index to seq_t */
    x = x + (seq_t)3;       /* x = n + 3 */
    return x * x - (seq_t)8;/* (n+3)^2 - 8 */
}
