/*
 * A171816  Smallest number of rank n in the poset of lunar numbers.
 * https://oeis.org/A171816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 - n (n < 9); a(n) = 10^(n-8) (n >= 9)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 9) {
        return (seq_t)(9 - n);
    }
    return pow10(n - 8);
}
