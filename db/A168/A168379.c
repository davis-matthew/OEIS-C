/*
 * A168379  a(n) = 4*n - 2*(-1)^n + 1.
 * https://oeis.org/A168379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2*(-1)^n + 1 (offset 1)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below the offset is irrelevant */
        return 0;

    seq_t x = (seq_t)n;          /* promote index to the sequence type    */
    if (x & 1)                   /* n is odd → (-1)^n = -1                */
        return 4*x + 3;          /* 4*n + 2 + 1 = 4*n + 3                 */
    else                         /* n is even → (-1)^n =  1              */
        return 4*x - 1;          /* 4*n - 2 + 1 = 4*n - 1                 */
}
