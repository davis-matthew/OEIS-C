/*
 * A210378  Number of 2 X 2 matrices with all terms in {0,1,...,n} and even
 *          trace.
 * https://oeis.org/A210378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 * floor(((n+1)^2 + 1)/2)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t np1 = (seq_t)n + 1;    /* convert index first */
    seq_t x   = np1 * np1;       /* (n+1)^2   */
    seq_t y   = (x + 1) / 2;     /* floor(((n+1)^2 + 1)/2) */
    return x * y;                /* final product */
}
