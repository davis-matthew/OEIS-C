/*
 * A043541  Number of distinct base-14 digits of n.
 * https://oeis.org/A043541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base‑14 digits of n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* not needed for offset 1, but harmless */
        return 0;

    uint64_t temp = (uint64_t)n; /* n comes in as idx_t */
    uint32_t mask = 0;           /* one bit per base‑14 digit */

    while (temp) {
        uint32_t d = temp % 14U;
        mask |= 1U << d;
        temp /= 14U;
    }

    /* count set bits in mask */
    uint32_t cnt = 0;
    while (mask) {
        cnt++;
        mask &= mask - 1;
    }

    return (seq_t)cnt;
}
