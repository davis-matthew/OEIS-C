/*
 * A048440  Take the first n numbers written in base 8, concatenate them, then
 *          convert from base 8 to base 10.
 * https://oeis.org/A048440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate the base‑8 representations of 1..n, *
 *            interpreted as a single base‑8 number
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* number of octal digits of i */
        idx_t tmp = i;
        idx_t digits = 0;
        while (tmp > 0)
        {
            tmp /= 8;
            digits++;
        }

        /* 8^digits in seq_t */
        seq_t pow8 = 1;
        for (idx_t k = 0; k < digits; ++k)
            pow8 *= 8;

        /* update the running value */
        val = val * pow8 + (seq_t)i;
    }
    return val;
}
