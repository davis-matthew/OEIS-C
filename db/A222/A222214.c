/*
 * A222214  In the number n, replace all (decimal) digits '0' with '5' and
 *          vice versa.
 * https://oeis.org/A222214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace every decimal digit 0 ↔ 5 in n's decimal
 *            representation
 * Range:     u8 n <= 205, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow10   = 1;            /* current power of 10 multiplier for digits */
    idx_t  temp   = n;

    /* Special handling for n == 0 : its decimal form is "0" -> "5" */
    if (temp == 0)
    {
        result = 5;
        return result;
    }

    while (temp > 0)
    {
        unsigned digit = temp % 10;   /* last decimal digit of temp (0..9) */
        temp /= 10;                   /* drop that digit */

        unsigned mapped;
        if (digit == 0)
            mapped = 5;
        else if (digit == 5)
            mapped = 0;
        else
            mapped = digit;

        /* Build the resulting number digit by digit */
        result += (seq_t)mapped * pow10;
        pow10 *= 10;
    }

    return result;
}
