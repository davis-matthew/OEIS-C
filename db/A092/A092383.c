/*
 * A092383  Sum of digits of n if n odd, else sum of digits of 2n.
 * https://oeis.org/A092383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of digits(n) if n odd else sum of digits(2n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if ((n & 1) == 0)
    {
        val += val;  /* even: 2n */
    }
    seq_t sum = 0;
    while (val > 0)
    {
        sum += val % 10;
        val /= 10;
    }
    return sum;
}
