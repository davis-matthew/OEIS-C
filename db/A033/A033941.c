/*
 * A033941  Number of ways A002808(n) can be properly factored into 2
 *          integers.
 * https://oeis.org/A033941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factor pairs (x,y) with * 1 < x ≤ y
 *            and x*y = m(n), where m(n) is the n-th composite * (non‑prime
 *            >1). This equals * (d(m) - 1)/2 if m is a square, * d(m)/2 - 1
 *            if m is not a square, * where d(m) is the divisor count of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9080 (2026-11-01)
 * Helpers:   is_prime, isqrt, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/isqrt.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Find the n-th composite number (composite = >1, not prime) */
    idx_t composite = 4;            /* first composite */
    idx_t count = 1;
    while (count < n) {
        ++composite;
        if (!is_prime(composite))
            ++count;
    }

    size_t d = num_divisors(composite);
    idx_t sqrt_val = isqrt(composite);
    if (sqrt_val * sqrt_val == composite)
        return (seq_t)((d - 1) / 2);
    else
        return (seq_t)(d / 2 - 1);
}
