/*
 * A216755  Digital root of the fifth power of Fibonacci(n).
 * https://oeis.org/A216755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digital_root(Fibonacci(n)^5)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Fibonacci numbers modulo 9 repeat with period 24 */
static const uint8_t fib_mod9[24] = {
    1, 1, 2, 3, 5, 8, 4, 3, 7, 1, 8, 0,
    8, 8, 7, 6, 4, 1, 5, 6, 2, 8, 1, 0
};

/* Digital root of r^5 for r = 0..8 (digital root of 0 is taken as 9) */
static const uint8_t dr_map[9] = {
    9, 1, 5, 9, 7, 2, 9, 4, 8
};

seq_t a(idx_t n)
{
    /* n ≥ 1; use the 24‑period of Fibonacci modulo 9 */
    idx_t idx = (n - 1) % 24;
    uint8_t r   = fib_mod9[(size_t)idx];
    uint8_t dr  = dr_map[(size_t)r];
    return (seq_t)dr;
}
