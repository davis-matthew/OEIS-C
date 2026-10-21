/*
 * A056963  Base 20 reversal of n (written in base 10).
 * https://oeis.org/A056963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse_base20(n)
 * Range:     u8 n <= 32, u16 n <= 8008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the base‑20 representation of n,
   reverses its digits, and interprets the result again in base‑20
   (i.e. as an ordinary integer).  */
/* For example: n = 22 → "12"₂₀ → reverse "21"₂₀ → 41. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t x = n;
    while (x > 0) {
        rev = rev * 20 + (seq_t)(x % 20);
        x /= 20;
    }
    return rev;
}
