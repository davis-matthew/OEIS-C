/*
 * A079789  a(n) = number of nonnegative integers m <= n having at least one
 *          digit in common with n.
 * https://oeis.org/A079789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = number of m ≤ n having at least one digit in common with
 *            n
 * Range:     u8 n <= 311, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint16_t digit_mask(idx_t x)
{
    /* 10‑bit mask: bit d is set if digit d occurs in x */
    if (x == 0) return 1U << 0;
    uint16_t mask = 0;
    while (x) {
        mask |= 1U << (x % 10);
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    const uint16_t mask_n = digit_mask(n);
    seq_t count = 0;
    for (idx_t m = 0; m <= n; ++m) {
        if (mask_n & digit_mask(m))
            ++count;
    }
    return count;
}
