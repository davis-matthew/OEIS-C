/*
 * A021962  Decimal expansion of 1/958.
 * https://oeis.org/A021962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of 1/958 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;                  /* remainder before multiplying by 10 */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t digit = (rem * 10) / 958;   /* digit at position i */
        rem = (rem * 10) % 958;              /* update remainder for next step */
        if (i == n) return (seq_t)digit;
    }
    /* In practice we never get here; the loop always returns. */
    return 0;
}
