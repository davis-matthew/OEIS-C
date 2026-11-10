/*
 * A088624  Index of the first occurrence of n in A068696.
 * https://oeis.org/A088624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least m >= 1 such that A068696(m) = n (search over m,
 *            counting primes among concat(m,k), k=1..m)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static idx_t A088624_concat(idx_t m, idx_t k)
{
    idx_t p = 1;
    idx_t t = k;
    do {
        p *= 10;
        t /= 10;
    } while (t);
    return m * p + k;
}

static idx_t A068696_a(idx_t m, idx_t limit)
{
    idx_t count = 0;
    for (idx_t k = 1; k <= m; ++k) {
        if (is_prime(A088624_concat(m, k))) {
            ++count;
            if (count > limit) return count;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    for (idx_t m = 1;; ++m) {
        if (A068696_a(m, n) == n) return (seq_t)m;
    }
}
