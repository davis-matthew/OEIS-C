/*
 * A021919  Decimal expansion of 1/915.
 * https://oeis.org/A021919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 915) mod 10 The function returns the nth
 *            decimal digit (0‑based) of 1/915
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for intermediate arithmetic to avoid overflow
       when seq_t might be as small as uint8_t. */
    idx_t rem = 1;              /* remainder before generating each digit */
    const idx_t den = 915;       /* denominator of the fraction */

    /* Generate digits one by one until the nth is reached. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                         /* shift remainder left by one decimal digit */
        seq_t digit = (seq_t)(rem / den);  /* integer part of the division */
        rem %= den;                        /* new remainder after removing the digit */

        if (i == n)
            return digit;                  /* nth digit found */
    }

    /* Control should never reach here; return 0 to silence warnings. */
    return 0;
}
