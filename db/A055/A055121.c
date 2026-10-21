/*
 * A055121  Base-11 complement of n (write n in base 11, then replace each
 *          digit with its base-11 negative).
 * https://oeis.org/A055121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = base‑11 complement of n: If n = Σ d_i·11^i in base‑11 (0
 *            ≤ d_i ≤ 10), then a(n) = Σ ((11 − d_i) mod 11)·11^i
 * Range:     u8 n <= 120, u16 n <= 1330, u32 n <= 1330, u64 n <= 1330,
 *            u128 n <= 1330, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n == 0, whose digit 0 maps to 0. */
    if (n == 0) return (seq_t)0;

    /* n has at most 64 bits so at most 64 base‑11 digits. */
    uint8_t digits[64];
    idx_t t = n;
    int pos = 0;
    while (t > 0) {
        digits[pos++] = (uint8_t)(t % 11);
        t /= 11;
    }

    seq_t res = (seq_t)0;
    for (int i = pos - 1; i >= 0; --i) {
        uint8_t comp = (uint8_t)((11 - digits[i]) % 11);
        res = res * (seq_t)11 + (seq_t)comp;
    }
    return res;
}
