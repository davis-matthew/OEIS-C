/*
 * A055962  n + reversal of base 12 digits of n (written in base 10).
 * https://oeis.org/A055962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + rev_base12(n) where rev_base12(n) is n with its
 *            base‑12 digits reversed
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8175 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed, otherwise ignored */

static inline seq_t rev_base12(idx_t n)
{
    seq_t rev = 0;
    idx_t temp = n;
    while (temp > 0) {
        uint64_t digit = temp % 12;          /* digit is small, < 12 */
        rev = rev * 12 + (seq_t)digit;       /* accumulate reversed digits */
        temp /= 12;
    }
    return rev;
}

seq_t a(idx_t n)
{
    return (seq_t)n + rev_base12(n);
}
