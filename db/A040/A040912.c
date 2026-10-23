/*
 * A040912  Continued fraction for sqrt(943).
 * https://oeis.org/A040912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 30; for n > 0, a(n) = 2 + floor((n+4)/6) – floor((n+7)/6)
 *            + 59*(floor((n+6)/6) – floor((n+5)/6))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;

    /* Compute n mod 6 */
    idx_t rem = n % (idx_t)6;   /* 6 as idx_t to avoid type mismatch */

    seq_t res = 2;  /* base value */

    /* floor((n+4)/6) – floor((n+7)/6) is -1 when n%6∈{0,1,5}, else 0 */
    if (rem == 0 || rem == 1 || rem == 5)
        res -= (seq_t)1;

    /* floor((n+6)/6) – floor((n+5)/6) is 1 when n%6==0, else 0 */
    if (rem == 0)
        res += (seq_t)59;

    return res;
}
