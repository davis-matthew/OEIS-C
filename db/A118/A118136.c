/*
 * A118136  a(n) = 2 + (2*n! mod n+1).
 * https://oeis.org/A118136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + (2*n! mod (n+1))
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* test whether x >= 2 is a prime number (trial division) */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (idx_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t np1 = n + 1;
    if (np1 > 2 && is_prime_idx(np1)) {
        return (seq_t)np1;
    }
    return (seq_t)2;
}
