/*
 * A077253  Sum of digits squared plus sum of digits of n.
 * https://oeis.org/A077253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of squares of the decimal digits of n) + (sum of the
 *            decimal digits of n)
 * Range:     u8 n <= 998, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0)
    {
        idx_t d = tmp % 10;          /* current decimal digit */
        seq_t ds = (seq_t)d;         /* promote to seq_t to keep precision */
        sum += ds * ds + ds;         /* add digit^2 + digit */
        tmp /= 10;                   /* move to next digit */
    }
    return sum;
}
