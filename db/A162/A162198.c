/*
 * A162198  Even cyclops numbers.
 * https://oeis.org/A162198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 0; for n > 1, a(n) is the (n-1)-th even integer whose
 *            decimal * representation has an odd number of digits and
 *            contains exactly one zero in the * centre position. Numbers are
 *            listed in increasing order
 * Range:     u8 n <= 9, u16 n <= 1837, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Index among the positive terms (excluding the initial 0). */
    idx_t m = n - 1;

    /* Find the length (number of decimal digits) of the target number. */
    idx_t len = 3;           /* we start with 3‑digit numbers */
    idx_t count;
    while (1)
    {
        /* count = 4 * 9^(len-2) */
        count = 4;
        for (idx_t i = 0; i < len - 2; ++i)
            count *= 9;

        if (m <= count)
            break;

        m -= count;
        len += 2;           /* move to the next odd length */
    }

    /* m is now the 1‑based index within the block of height 'len'. */
    idx_t pos     = m;             /* 1‑based within this block */
    idx_t pos0    = pos - 1;       /* 0‑based for base conversions */
    seq_t last_digit = (seq_t)((pos0 % 4) + 1) * 2; /* 2,4,6,8 */
    pos0 /= 4;

    /* Number of varying digits (all except the centre zero and the last digit). */
    idx_t var_digits = len - 2;

    /* Store those digits, each from 1 to 9. */
    unsigned int digits[64] = {0};
    for (int i = (int)var_digits - 1; i >= 0; --i)
    {
        digits[i] = (unsigned int)(pos0 % 9 + 1);
        pos0      /= 9;
    }

    /* Construct the number digit by digit. */
    seq_t value = 0;
    idx_t centre = (len - 1) / 2;
    idx_t di     = 0;            /* index into digits array */

    for (idx_t j = 0; j < len; ++j)
    {
        value = value * 10;

        if (j == centre)
            ; /* centre zero */
        else if (j == len - 1)
            value += last_digit;
        else
        {
            value += digits[di];
            ++di;
        }
    }

    return value;
}
