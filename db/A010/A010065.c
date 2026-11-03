/*
 * A010065  a(n+1) = a(n) + sum of digits in base 4 representation of a(n),
 *          with a(0) = 1.
 * https://oeis.org/A010065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) + sum of base-4 digits of a(n), a(0) = 1
 * Range:     u8 n <= 47, u16 n <= 5831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8631 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the sum of the base‑4 digits of x */
static seq_t sum_digits_base4(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        sum += x & 3;   /* digit in base 4 (x % 4) */
        x >>= 2;        /* integer divide by 4 (x /= 4) */
    }
    return sum;
}

/* Returns the nth term of OEIS sequence A010065 */
seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) = 1 */
    if (n == 0)
        return val;

    for (idx_t i = 0; i < n; ++i)
        val += sum_digits_base4(val);

    return val;
}
