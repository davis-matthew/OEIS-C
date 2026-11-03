/*
 * A187333  a(n) = floor(n/5) + floor(2n/5) + floor(3n/5) + floor(4n/5).
 * https://oeis.org/A187333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/5)+floor(2n/5)+floor(3n/5)+floor(4n/5)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: compute floor(n * mul / div) without overflow in idx_t */
static inline seq_t mul_div_floor(idx_t n, idx_t mul, idx_t div)
{
    idx_t q = n / div;
    idx_t r = n % div;
    return (seq_t)q * (seq_t)mul + (seq_t)((r * mul) / div);
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    total += mul_div_floor(n, 1, 5);
    total += mul_div_floor(n, 2, 5);
    total += mul_div_floor(n, 3, 5);
    total += mul_div_floor(n, 4, 5);
    return total;
}
