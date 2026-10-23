/*
 * A125960  Sum of [n mod Sum_of_digits(n)] and [Sum_of_digits(n) mod n].
 * https://oeis.org/A125960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n mod s(n)) + (s(n) mod n) where s(n) = sum of decimal
 *            digits of n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sum of decimal digits of a 64‑bit integer. */
static uint32_t sum_digits_uint64(uint64_t x)
{
    uint32_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* sum of digits of n (fits in a 32‑bit unsigned integer) */
    uint32_t sum = sum_digits_uint64((uint64_t)n);

    /* part1: n mod sum (sum >= 1 for n >= 1) */
    idx_t part1 = n % sum;

    /* part2: sum mod n */
    idx_t part2 = sum % n;

    /* sum of the two parts fits in idx_t; cast to seq_t for return */
    return (seq_t)(part1 + part2);
}
