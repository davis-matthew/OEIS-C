/*
 * A160700  a(n) = if n<16 then n else a(floor(n/16)) XOR (n mod 16).
 * https://oeis.org/A160700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(floor(n/16)) XOR (n mod 16), a(n)=n for n<16
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent:  a(n) is the xor of all hexadecimal digits of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n >= 16) {
        res ^= (seq_t)(n & 15);   /* n mod 16 */
        n >>= 4;                  /* n = floor(n/16) */
    }
    res ^= (seq_t)n;              /* XOR with the remaining low digit */
    return res;
}
