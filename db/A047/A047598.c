/*
 * A047598  Numbers that are congruent to {3, 4, 5} mod 8.
 * https://oeis.org/A047598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=4, a(3)=5, a(n)=8*floor((n-1)/3)+ (3 + ((n-1)%3))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n>=1 is required by the problem statement */
    seq_t nm1 = (seq_t)n - ((seq_t)1);
    seq_t m   = nm1 / ((seq_t)3);   /* floor((n-1)/3)   */
    seq_t r   = nm1 % ((seq_t)3);   /* (n-1) mod 3      */
    return (seq_t)8 * m + ((seq_t)3 + r);
}
