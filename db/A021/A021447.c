/*
 * A021447  Decimal expansion of 1/443.
 * https://oeis.org/A021447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/443) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the decimal expansion of 1/443, 0-indexed after the decimal point. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Long‑division of 1/443.  The remainder always stays below 443,
       so seq_t is sufficient. */
    seq_t r = 1;
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;          /* bring down the next digit */
        seq_t digit = r / 443;
        r = r % 443;
        if (i == n)
            return digit;    /* digit at position n */
    }
    return 0;                 /* never reached */
}
