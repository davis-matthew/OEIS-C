/*
 * A101550  Lopsided (or biased) numbers: numbers n such that the largest
 *          prime factor of n is > 2*sqrt(n).
 * https://oeis.org/A101550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lopsided numbers are n such that (largest prime factor of n)^2 >
 *            4*n
 * Range:     u8 n <= 4, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We search for the n‑th such number by brute force.                              */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t num = 5;                /* first term is 5 */
    while (1) {
        seq_t p = greatest_prime_factor(num);
        seq_t lhs = p * p;
        seq_t rhs = 4 * num;
        if (lhs > rhs) {
            ++cnt;
            if (cnt == n)
                return num;
        }
        ++num;
    }
}
