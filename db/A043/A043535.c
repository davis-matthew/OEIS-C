/*
 * A043535  Number of distinct base-8 digits of n.
 * https://oeis.org/A043535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of distinct base‑8 digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base‑8 representation of 0 is "0" → one distinct digit. */
    if (n == 0)
        return (seq_t)1;

    uint8_t mask = 0;
    idx_t t = n;
    while (t) {
        uint8_t d = t % 8;
        mask |= 1u << d;
        t /= 8;
    }

    int cnt = 0;
    uint8_t m = mask;
    while (m) {
        m &= m - 1;   /* clears the lowest set bit */
        cnt++;
    }
    return (seq_t)cnt;
}
