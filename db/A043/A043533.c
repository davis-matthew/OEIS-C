/*
 * A043533  Number of distinct base-6 digits of n.
 * https://oeis.org/A043533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base‑6 digits of n (n>=1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint8_t mask = 0;
    idx_t x = n;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % (idx_t)6);
        mask |= (1u << d);
        x /= (idx_t)6;
    }
    unsigned int cnt = 0;
    uint8_t tmp = mask;
    while (tmp) {
        cnt += tmp & 1u;
        tmp >>= 1;
    }
    return (seq_t)cnt;
}
