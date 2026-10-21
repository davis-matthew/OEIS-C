/*
 * A089057  n! divided by prime whose index is the integer part of log(n).
 * https://oeis.org/A089057
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / prime( floor( ln(n) ) ), n ≥ 3 Here prime(k) denotes
 *            the k‑th prime (prime(1)=2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Definition only for n ≥ 3 */
    if (n < 3)
        return 0;

    /* m = ⌊ ln(n) ⌋  (natural logarithm) */
    idx_t m = (idx_t)floor(log((long double)n));
    if (m == 0)
        return 0;          /* cannot happen for n >= 3, but keep safe */

    /* retrieve the m‑th prime */
    seq_t prime = get_prime(m);   /* get_prime is provided by the framework */

    /* compute n!  (no caching – simple iterative product) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* divide by the prime and return */
    return fact / prime;
}
