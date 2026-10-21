/*
 * A218215  Product of the nonzero digits (in base 10) of n^4.
 * https://oeis.org/A218215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of non‑zero decimal digits of n^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^4 in seq_t to avoid intermediate overflows on built‑in types */
    seq_t power = (seq_t)n;
    power *= (seq_t)n;   /* n^2 */
    power *= (seq_t)n;   /* n^3 */
    power *= (seq_t)n;   /* n^4 */

    /* Multiply the non‑zero digits of power */
    seq_t prod = 1;
    while (power > 0) {
        seq_t digit = power % (seq_t)10;
        if (digit != 0)
            prod *= digit;
        power /= (seq_t)10;
    }
    return prod;
}
