/*
 * A078770  a(n) = the least positive integer k such that k^2 + k + 2*n - 1 is
 *          prime.
 * https://oeis.org/A078770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that k² + k + 2 n – 1 is
 *            prime
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is not used but we guard anyway */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t base = nn * (seq_t)2 - (seq_t)1;  /* 2*n – 1 */
    seq_t k    = (seq_t)1;

    for (;;) {
        seq_t val = k * k + k + base;
        if (is_prime_big(val)) return k;
        ++k;
    }
}
