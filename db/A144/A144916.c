/*
 * A144916  Integers k for which |A144912| attains a new maximal odd value.
 * https://oeis.org/A144916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=7, a(n) = (2*(n-1))^2 for n≥3
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    seq_t r = (seq_t)(n - 1);   /* r = n-1 as seq_t */
    r = r * (seq_t)2;           /* r = 2*(n-1) */
    return r * r;               /* return r^2 */
}
