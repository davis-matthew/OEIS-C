/*
 * A117364  a(n) = largest prime less than the largest prime dividing n (or 1
 *          if there is no such prime).
 * https://oeis.org/A117364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime less than the largest prime dividing n, or
 *            1 if there is no such prime
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)                      /* 1 has no prime factors */
        return 1;

    uint64_t p = largest_prime_factor((uint64_t)n);   /* largest prime factor of n */
    if (p <= 2)                      /* no smaller prime exists */
        return 1;

    idx_t prev = prev_prime((idx_t)p);   /* largest prime < p */
    if (prev < 2)
        return 1;
    return (seq_t)prev;
}
