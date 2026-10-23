/*
 * A122638  a(n) = {n+1}_n.
 * https://oeis.org/A122638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = {n+1}_n (interpret decimal digits of n+1 as a base‑n
 *            number)
 * Range:     u8 n <= 50, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* safety for index 0, not used in the series */

    /* Convert n+1 to its decimal digits (least‑significant first). */
    idx_t tmp = n + 1;
    uint8_t digits[20];            /* sufficient for 64‑bit values */
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Interpret these digits as a base‑n integer. */
    seq_t result = 0;
    seq_t base = (seq_t)n;
    for (size_t i = 0; i < len; ++i) {
        result = result * base + (seq_t)digits[len - 1 - i];
    }
    return result;
}
