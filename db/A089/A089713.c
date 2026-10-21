/*
 * A089713  a(n) is the largest prime that is the concatenation of n and m for
 *          some m < n, or 0 if no such prime exists.
 * https://oeis.org/A089713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the largest prime that can be written as the concatenation
   of n and some m with 1 ≤ m < n; returns 0 if no such prime exists. 
   n is 1‑based as per OEIS. */
#include <stdbool.h>   /* for bool (unused but required by some utilities) */
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 2) return 0;

    /* Search for the largest m such that concat(n,m) is prime */
    idx_t m = n;
    while (m > 1) {
        --m;                       /* start with m = n-1 */
        seq_t val = concat_parts(n, m);
        if (is_prime_big(val)) return val;
    }
    return 0;                      /* no prime found */
}
