/*
 * A158586  a(n) = 34*n^2 + 1.
 * https://oeis.org/A158586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2, then multiply by 34, and add 1.
       All arithmetic is done in seq_t to avoid overflow for
       the ranges used by the tests (n <= 10000).  For larger n
       the value may overflow seq_t, but the sequence definition
       only requires correctness for values that fit in seq_t. */
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2   */
    return nn * 34 + 1;
}
