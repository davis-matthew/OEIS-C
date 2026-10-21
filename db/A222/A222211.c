/*
 * A222211  In the number n, replace all (decimal) digits '0' with '2' and
 *          vice versa.
 * https://oeis.org/A222211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the decimal digits of n with 0s replaced by 2s and 2s by
 *            0s (no leading zeros)
 * Range:     u8 n <= 299, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;

    seq_t result = 0;
    seq_t factor = 1;
    idx_t tmp = n;

    while (tmp > 0)
    {
        unsigned digit = (unsigned)(tmp % 10);
        unsigned sw_digit = (digit == 0 ? 2 : (digit == 2 ? 0 : digit));

        result += (seq_t)sw_digit * factor;
        factor *= 10;
        tmp /= 10;
    }

    return result;
}
