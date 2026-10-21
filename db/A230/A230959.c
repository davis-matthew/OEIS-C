/*
 * A230959  If n is pandigital then 0 else (digits not occurring in decimal
 *          representation of n, arranged in decreasing order).
 * https://oeis.org/A230959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 if the decimal representation of n contains all digits 0–9;
   otherwise a(n) is the integer obtained by writing the missing digits
   in decreasing order (e.g. n = 0 → 987654321, n = 12 → 98765430). */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute which digits appear in n */
    uint32_t mask = 0;
    idx_t x = n;
    if (x == 0)                /* digit 0 is present */
        mask |= 1u << 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }

    /* If all ten digits are present, return 0 */
    if (mask == 0x3FF) /* binary 1111111111 */
        return (seq_t)0;

    /* Build the number from missing digits, descending from 9 to 0 */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        if (!(mask & (1u << d))) {
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}
