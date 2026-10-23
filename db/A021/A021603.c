/*
 * A021603  Decimal expansion of 1/599.
 * https://oeis.org/A021603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of the expansion of 1/599.
   The sequence is a(0) = 0, a(1) = 0, a(2) = 1, …  (digits after the decimal point). */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;      /* remainder after the decimal point (initially 1). */
    uint64_t digit = 0;  /* current digit */

    /* To obtain digit a(n) we need n+1 steps of the long‑division algorithm. */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;               /* bring down the next zero */
        digit = r / 599;       /* integer part is the next decimal digit */
        r %= 599;              /* new remainder for the next step */
    }

    return (seq_t)digit;        /* cast to the build‑specific integer type */
}
