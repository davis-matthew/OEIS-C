/*
 * A158669  a(n) = 900*n^2 - 30.
 * https://oeis.org/A158669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 900*n^2 - 30
 * Range:     u8 none, u16 n <= 8, u32 n <= 2184, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is defined for n >= 1 */
    return (seq_t)900 * (seq_t)n * (seq_t)n - (seq_t)30;
}
