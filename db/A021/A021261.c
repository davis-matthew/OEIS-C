/*
 * A021261  Decimal expansion of 1/257.
 * https://oeis.org/A021261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point of 1/257 * (0‑based
 *            indexing: a(0) = first digit, a(1) = second digit, etc.)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The decimal expansion of 1/257 is obtained by long division.
       We keep the remainder r (initially 1) and at each step compute
       the next digit as r*10 / 257.  This works for any unsigned integer
       type seq_t large enough to hold values up to 257. */
    seq_t r = 1;      /* current remainder */
    seq_t digit = 0;  /* digit produced in the current step */
    /* We need n+1 divisions to get the digit at index n (0‑based). */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;             /* bring down next decimal place */
        digit = r / 257;     /* next digit */
        r = r % 257;         /* new remainder */
    }
    return digit;
}
