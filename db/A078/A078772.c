/*
 * A078772  a(n) = phi(n-p) where p is largest prime < n, a(1) = a(2) = 1 by
 *          convention.
 * https://oeis.org/A078772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n - p) where p is the largest prime < n, with a(1) =
 *            a(2) = 1. Uses prev_prime() and phi() helpers
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 9885 (2026-11-01)
 * Helpers:   is_prime, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/totient.h"

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
    if (n <= 2)            /* a(1) and a(2) are defined as 1 */
        return (seq_t)1;

    idx_t p = prev_prime(n);    /* largest prime < n */
    idx_t diff = n - p;         /* n - p  ≥ 1 for n ≥ 3 */

    return totient(diff);           /* φ(n - p) */
}
