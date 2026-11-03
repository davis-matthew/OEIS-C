/*
 * A108139  n to the power of the largest prime divisor of n.
 * https://oeis.org/A108139
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(largest prime divisor of n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Return the largest prime factor of n (n >= 2). */
static idx_t largest_prime_factor(idx_t n)
{
    idx_t largest = 1;

    if (n % 2 == 0) {
        largest = 2;
        while (n % 2 == 0) n /= 2;
    }

    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            largest = p;
            while (n % p == 0) n /= p;
        }
    }

    if (n > 1)   /* n itself is a prime factor */
        largest = n;

    return largest;
}

/* OEIS A108139: n^(largest prime divisor of n) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2, return 0 */
    idx_t lp   = largest_prime_factor(n);
    return ipow_idx((seq_t)n, lp);
}
