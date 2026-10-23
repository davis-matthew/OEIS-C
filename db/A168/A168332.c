/*
 * A168332  a(n) = 6 + 7 * floor((n-1)/2).
 * https://oeis.org/A168332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 + 7 * floor((n-1)/2)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is index >= 1; convert to seq_t and use simple integer arithmetic */
    seq_t nn = (seq_t)n;
    seq_t k  = (nn - 1) / 2;   /* floor((n-1)/2) */
    return 6 + 7 * k;
}
