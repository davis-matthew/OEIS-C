/*
 * A051329  A generalized Thue-Morse sequence.
 * https://oeis.org/A051329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A053735(n) mod 4, where A053735(n) = sum of base-3 digits
 *            of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A053735_a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t x = n; x != 0; x /= 3)
        sum += x % 3;
    return sum;
}

seq_t a(idx_t n)
{
    return A053735_a(n) % 4;
}
