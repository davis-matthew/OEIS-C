/*
 * A135042  a(n) = 5*(n - 1) for n >= 2; a(0) = 1, a(1) = 2.
 * https://oeis.org/A135042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*(n-1) for n >= 2; a(0) = 1, a(1) = 2
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    /* For n >= 2 */
    return (seq_t)(5 * ((seq_t)n - 1));
}
