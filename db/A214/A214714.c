/*
 * A214714  Least m>0 such that prime(n)-m and n-m are relatively prime.
 * https://oeis.org/A214714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the A214714 definition:
   a(n) = smallest m>0 such that gcd( prime(n)-m , n-m ) = 1
   In the code we use the identity gcd( prime(n)-m , n-m )
   = gcd( prime(n)-n , n-m ), so the problem reduces to
   finding the least m>0 such that gcd( |prime(n)-n| , n-m ) = 1.
*/

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* p = prime(n) – the n-th prime number */
    uint64_t p = (uint64_t)get_prime(n);

    /* g = |p - n|   (non‑negative int of seq_t type) */
    seq_t g;
    if (p >= n)
        g = (seq_t)(p - n);
    else
        g = (seq_t)(n - p);

    /* Find the largest k < n with gcd(g, k) == 1.
       Then m = n - k is the desired value. */
    for (idx_t k = n - 1; k > 0; --k) {
        if (gcd(g, (seq_t)k) == 1)
            return (seq_t)(n - k);
    }

    /* If no k>0 works, use k = 0 (m = n).
       This can happen only when g == 1. */
    if (gcd(g, (seq_t)0) == 1)
        return (seq_t)n;

    /* Should never reach here – return n as a last resort. */
    return (seq_t)n;
}
