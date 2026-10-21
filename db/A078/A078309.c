/*
 * A078309  Numbers that are congruent to {1, 4, 7} mod 10.
 * https://oeis.org/A078309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3*mod(n-1,3) + 10*floor((n-1)/3)
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t sn  = (seq_t)n;    /* convert index to seq_t */
    const seq_t n1  = sn - 1;      /* n - 1 */
    const seq_t r   = n1 % 3;      /* (n-1) mod 3 */
    const seq_t q   = n1 / 3;      /* floor((n-1)/3) */
    return 1 + 3 * r + 10 * q;
}
