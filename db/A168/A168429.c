/*
 * A168429  a(n) = 4^n mod 11.
 * https://oeis.org/A168429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 4^n modulo 11.  Handles all n >= 0. */
seq_t a(idx_t n)
{
    seq_t result = 1;              /* 4^0 ≡ 1 (mod 11) */
    seq_t base   = 4 % 11;         /* 4 mod 11 = 4 */
    idx_t exp    = n;

    while (exp) {
        if (exp & 1)
            result = (seq_t)((seq_t)result * base % 11);
        base = (seq_t)((seq_t)base * base % 11);
        exp >>= 1;
    }
    return result;
}
