/*
 * A103173  Numbers k such that the decimal digits of k are not present in
 *          k^2, k^3, or k^4.
 * https://oeis.org/A103173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: search over k to find the n‑th integer k such that none of the decimal digits of k occur in k^2, k^3, or k^4. */

#include <stdbool.h>
#include <stdint.h>

static inline uint16_t digit_mask(seq_t x)
{
    uint16_t m = 0;
    while (x != 0)
    {
        uint8_t d = (uint8_t)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

static inline bool good_k(seq_t k)
{
    uint16_t km = digit_mask(k);

    seq_t k2 = k * k;
    if (km & digit_mask(k2)) return false;

    seq_t k3 = k2 * k;
    if (km & digit_mask(k3)) return false;

    seq_t k4 = k3 * k;
    return !(km & digit_mask(k4));
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t cnt = 0;
    seq_t k = 1;
    while (cnt < n)
    {
        if (good_k(k))
        {
            ++cnt;
            if (cnt == n) return k;
        }
        ++k;
    }
    return 0; /* unreachable */
}
