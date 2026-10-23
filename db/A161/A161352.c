/*
 * A161352  a(n) = n + R(n) + digsum(n) + product of the digits of n.
 * https://oeis.org/A161352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + rev(n) + digit_sum(n) + digit_product(n)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t rev = 0;

    if (t == 0) /* product of digits of 0 is defined as 0 */
    {
        prod = 0;
    }
    else
    {
        while (t > 0)
        {
            idx_t d = t % 10;
            rev = rev * 10 + d;
            sum += (seq_t)d;
            prod *= (seq_t)d;
            t /= 10;
        }
    }

    seq_t result = (seq_t)n;               /* n itself */
    result += (seq_t)rev;                  /* reverse of n */
    result += sum;                         /* sum of its digits */
    result += prod;                        /* product of its digits */

    return result;
}
