/*
 * A157730  a(n) = 441*n^2 - 488*n + 135.
 * https://oeis.org/A157730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n^2 - 488*n + 135
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 3121, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to seq_t before any arithmetic */
    seq_t nn = (seq_t)n;
    return 441 * nn * nn - 488 * nn + 135;
}
