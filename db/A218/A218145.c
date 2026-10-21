/*
 * A218145  Product of the nonzero digits (in base 10) of n^3.
 * https://oeis.org/A218145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of nonzero digits of n³ in base 10
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the sequence value for index n (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Compute n³ using 64‑bit arithmetic (enough for n up to 10⁴). */
    unsigned long long cube = (unsigned long long)n * n * n;

    unsigned long long temp = cube;
    seq_t prod = 1;          /* product of the non‑zero digits */

    while (temp > 0) {
        unsigned int digit = temp % 10;
        if (digit != 0) {
            prod *= (seq_t)digit;
        }
        temp /= 10;
    }
    return prod;
}
