/*
 * A010764  a(n) = floor(n/2) mod floor(n/3).
 * https://oeis.org/A010764
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) mod floor(n/3)
 * Range:     u8 n <= 1531, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* n/3 is zero for n=1,2 */
    idx_t half  = n / 2;                /* floor(n/2) */
    idx_t third = n / 3;                /* floor(n/3) */
    return (seq_t)(half % third);       /* modulo in idx_t, cast to seq_t */
}
