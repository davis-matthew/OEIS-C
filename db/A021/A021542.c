/*
 * A021542  Decimal expansion of 1/538.
 * https://oeis.org/A021542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the expansion of
 *            1/538
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by performing a long‑division of 1 by 538 up to the (n+1)th step. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 64‑bit unsigned integer for the remainder – it always stays <538 */
    uint64_t rem = 1;         /* remainder after the previous digit */
    seq_t digit = (seq_t)0;   /* digit to be returned */

    /* Perform the division up to the requested index. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                     /* bring down next 0 */
        digit = (seq_t)(rem / 538ULL);    /* next decimal digit */
        rem  %= 538ULL;                   /* new remainder */
    }
    return digit;
}
