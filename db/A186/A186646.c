/*
 * A186646  Every fourth term of the sequence of natural numbers 1,2,3,4,...
 *          is halved.
 * https://oeis.org/A186646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(4n)=2n; a(4n+1)=4n+1; a(4n+2)=4n+2; a(4n+3)=4n+3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 3) == 0)
        return (seq_t)(n >> 1);  /* n is divisible by 4, halve it */
    else
        return (seq_t)n;         /* otherwise keep n */
}
