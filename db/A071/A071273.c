/*
 * A071273  Concatenation of R(n) (A004086) and n, omitting leading 0's.
 * https://oeis.org/A071273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(A004086(n), n) where A004086(n) is the decimal
 *            reverse of n with leading zeros removed
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;
    seq_t pow10 = 1;
    idx_t tmp = n;

    /* compute reverse of n (leading zeros automatically dropped) */
    while (tmp > 0)
    {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* compute 10^(digits of n) */
    tmp = n;
    while (tmp > 0)
    {
        pow10 *= 10;
        tmp /= 10;
    }

    /* concatenate: rev followed by n */
    return rev * pow10 + (seq_t)n;
}
