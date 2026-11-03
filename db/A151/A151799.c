/*
 * A151799  Version 2 of the "previous prime" function: largest prime < n.
 * https://oeis.org/A151799
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = largest prime < n (previous prime function)
 * Range:     u8 n <= 257, u16 n <= 65537, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 91 terms through n = 9947 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) = 2) */

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
    if (n < 3)
        return 0;          /* behaviour below offset is irrelevant */
    return (seq_t)prev_prime(n);
}
