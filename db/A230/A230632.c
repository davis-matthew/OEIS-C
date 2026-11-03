/*
 * A230632  Number of integers m such that m + (sum of digits in base-4
 *          representation of m) = n.
 * https://oeis.org/A230632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ m ≥ 0 | m + sumdigits4(m) = n } (sumdigits4(m) = sum
 *            of digits of m in base‑4)
 * Range:     u8 n <= 665, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9335 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the sum of base‑4 digits of x, as a seq_t. */
static seq_t sumdigits4(idx_t x)
{
    seq_t s = 0;
    while (x) {
        s += (seq_t)(x & 3U);   /* x mod 4 */
        x >>= 2;                /* divide by 4 */
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t m = 0; m <= n; ++m) {
        seq_t val = (seq_t)m + sumdigits4(m);   /* m + digit sum */
        if (val == (seq_t)n)
            ++cnt;
    }
    return cnt;
}
