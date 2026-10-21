/*
 * A057524  Number of 3 X n binary matrices without singleton columns up to
 *          row and column permutations.
 * https://oeis.org/A057524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( ((n+2)*(n+6)/12)^2 ) = floor( ((n+2)*(n+6))^2 /
 *            144 )
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 439, u64 n <= 1374,
 *            u128 n <= 1374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote the index to seq_t to perform arithmetic in the result type. */
    seq_t n0 = (seq_t)n;
    seq_t p  = (n0 + 2) * (n0 + 6);   /* (n+2)*(n+6) */
    return (p * p) / 144;              /* floor( p^2 / 144 ) */
}
