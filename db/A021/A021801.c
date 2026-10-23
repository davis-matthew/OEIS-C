/*
 * A021801  Decimal expansion of 1/797.
 * https://oeis.org/A021801
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

/* Decimal expansion of 1/797.  a(n) is the n‑th digit after the decimal point. */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;      /* current remainder before multiplying by 10 */
    uint64_t d = 0;      /* digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;              /* shift one decimal place */
        d = r / 797;          /* next digit */
        r %= 797;             /* new remainder */
    }
    return (seq_t)d;           /* d is always 0‑9, fits in any seq_t */
}
