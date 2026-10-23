/*
 * A168233  a(n) = 3*n - a(n-1) - 1 for n>0, a(1)=1.
 * https://oeis.org/A168233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3*(-1)^n + 1)/4 (n ≥ 1)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t six_n = (seq_t)6 * n;
    if (n & 1)            /* n is odd: numerator = 6*n – 2 */
        return (six_n - (seq_t)2) / 4;
    else                   /* n is even: numerator = 6*n + 4 */
        return (six_n + (seq_t)4) / 4;
}
