/*
 * A040958  Continued fraction for sqrt(990).
 * https://oeis.org/A040958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=31, a(n)=period[(n-1)%4] for n≥1, period = {2,6,2,62}
 *            (continued fraction of √990)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of the repeating block after the integer part */
    static const seq_t period[4] = { (seq_t)2, (seq_t)6, (seq_t)2, (seq_t)62 };

    if (n == 0)
        return (seq_t)31;

    /* since the period length 4 is a power of two, use a mask for speed */
    idx_t idx = (n - 1) & (idx_t)3;   /* (n-1) mod 4 */
    return period[idx];
}
