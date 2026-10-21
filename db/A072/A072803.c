/*
 * A072803  a(n) is n written in base n mod 10, or 0 if n mod 10 = 0.
 * https://oeis.org/A072803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n written in base (n mod 10) as a decimal integer, or 0
 *            if n mod 10 = 0
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the base would be 0, return 0 */
    if (n % 10 == 0)
        return 0;

    /* Determine the base (1 through 9) */
    idx_t base = n % 10;

    /* Base 1: unary representation – a string of n ones */
    if (base == 1) {
        seq_t result = 0;
        for (idx_t i = 0; i < n; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Other bases: standard base conversion */
    idx_t temp = n;
    /* The maximum number of digits for any n <= 10000 and base >= 2
       is less than 64 (2^6 = 64 > 10000). */
    unsigned char digits[64];
    int nd = 0;

    while (temp > 0) {
        digits[nd++] = (unsigned char)(temp % base);
        temp /= base;
    }

    seq_t result = 0;
    for (int i = nd - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
