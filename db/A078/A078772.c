/*
 * A078772  a(n) = phi(n-p) where p is largest prime < n, a(1) = a(2) = 1 by
 *          convention.
 * https://oeis.org/A078772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n - p) where p is the largest prime < n, with a(1) =
 *            a(2) = 1. Uses prev_prime() and phi() helpers
 * Range:     u8 n <= 9885, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   phi, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n <= 2)            /* a(1) and a(2) are defined as 1 */
        return (seq_t)1;

    idx_t p = prev_prime(n);    /* largest prime < n */
    idx_t diff = n - p;         /* n - p  ≥ 1 for n ≥ 3 */

    return phi(diff);           /* φ(n - p) */
}
