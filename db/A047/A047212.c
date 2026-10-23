/*
 * A047212  Numbers that are congruent to {0, 2, 4} mod 5.
 * https://oeis.org/A047212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-3)/3)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (5*n-3)/3 using integer arithmetic, all in seq_t */
    seq_t val = (seq_t)5 * n;  /* 5*n */
    val -= (seq_t)3;           /* 5*n - 3 */
    val /= (seq_t)3;           /* floor division by 3 */
    return val;
}
