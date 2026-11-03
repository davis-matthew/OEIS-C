/*
 * A049716  a(n) = 2*n + 1 - prevprime(2*n + 1).
 * https://oeis.org/A049716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 - prevprime(2*n + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9331 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

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
    /* Compute m = 2*n + 1 */
    idx_t m = 2 * n + 1;

    /* Find the previous prime < m */
    idx_t pp = prev_prime(m);

    /* Return the difference */
    return (seq_t)(m - pp);
}
