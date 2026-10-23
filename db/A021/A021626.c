/*
 * A021626  Decimal expansion of 1/622.
 * https://oeis.org/A021626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit after point of 1/622 i.e. a(n) =
 *            floor(10^(n+1)/622) % 10 The function builds the digits on
 *            demand and caches already generated digits for future queries
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache to store already computed digits */
    static seq_t *digits = NULL;
    static idx_t digits_len = 0;  /* number of digits stored */
    static idx_t rem = 1;         /* current remainder after last digit */

    /* initialize cache on first call */
    if (digits == NULL)
    {
        digits_len = 0;
        digits = (seq_t*)malloc(sizeof(seq_t));
        if (!digits) return 0;    /* out‑of‑memory; behaviour unspecified */
        rem = 1;
    }

    /* extend cache until it contains digit a(n) */
    if (n >= digits_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(digits, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* out‑of‑memory; behaviour unspecified */
        digits = tmp;

        for (idx_t i = digits_len; i <= n; ++i)
        {
            /* next digit computation: (rem * 10) / 622 */
            rem = rem * 10;
            seq_t digit = rem / 622;
            rem = rem % 622;
            digits[i] = digit;
        }
        digits_len = new_len;
    }

    return digits[n];
}
