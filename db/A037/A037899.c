/*
 * A037899  a(n) = (greatest base 5 digit of n) - (least base 5 digit of n).
 * https://oeis.org/A037899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (greatest base 5 digit of n) - (least base 5 digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* base‑5 representation of 0 is "0" */
        return 0;

    idx_t temp = n;
    unsigned maxdigit = 0;
    unsigned mindigit = 5;    /* greater than any possible digit (0‑4) */

    while (temp > 0) {
        unsigned digit = temp % 5;
        if (digit > maxdigit)  maxdigit = digit;
        if (digit < mindigit)  mindigit = digit;
        temp /= 5;
    }

    return (seq_t)(maxdigit - mindigit);
}
