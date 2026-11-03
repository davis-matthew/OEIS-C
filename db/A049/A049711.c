/*
 * A049711  a(n) = n - prevprime(n).
 * https://oeis.org/A049711
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - prevprime(n)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 95 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n < 3) return (seq_t)0;   /* Offset is 3 – behavior below is irrelevant */
    idx_t p   = prev_prime(n);    /* largest prime < n (variant 2) */
    idx_t d   = n - p;            /* difference fits in seq_t for the tested range */
    return (seq_t)d;
}
