/*
 * A240454  Smallest prime divisors of the palindromes with an even number of
 *          digits.
 * https://oeis.org/A240454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest prime divisor of concat_parts(n, rev(n))
 * Range:     u8 n <= 1681, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9596 (2026-11-01)
 * Helpers:   num_digits_idx, reverse_digits_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/smallest_prime_factor.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for offsets below 1 */

    int d = num_digits_idx(n);                         /* number of decimal digits of n */
    seq_t ten_power = pow10(d);                 /* 10^d as seq_t */
    uint64_t val = (uint64_t)n * (uint64_t)ten_power + (uint64_t)reverse_digits_idx(n);

    uint64_t spf = smallest_prime_factor(val);
    return (seq_t)spf;
}
