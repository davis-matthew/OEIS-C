/*
 * A021325  Decimal expansion of 1/321.
 * https://oeis.org/A021325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of 1/321: a(n) = nth decimal digit of 1/321. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute by long division: start with remainder 1 (the
       numerator of 1/321).  Each step multiplies the remainder
       by 10, the quotient is the next decimal digit, and the
       remainder is updated modulo 321. */
    uint64_t rem = 1;       /* current remainder (0 ≤ rem < 321) */
    seq_t digit = 0;        /* value to return */

    for (idx_t i = 0; i <= n; ++i) {
        uint64_t t = rem * 10ULL;
        digit = (seq_t)(t / 321ULL); /* digit for this position   */
        rem   = t % 321ULL;          /* remainder for next step   */
    }

    return digit;  /* a(n) is the digit obtained after the nth step */
}
