/*
 * A214490  Smallest integer k such that prime(n+1) = floor(prime(n)/sin(k)).
 * https://oeis.org/A214490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer k such that * prime(n+1) = ⌊
 *            prime(n) / sin(k) ⌋, k in radians
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* Retrieve the nth and (n+1)th primes. */
    uint32_t p = get_prime(n);       /* prime(n) */
    uint32_t q = get_prime(n + 1);   /* prime(n+1) */

    /* Search for the smallest integer k ≥ 1 satisfying the condition. */
    for (idx_t k = 1;; ++k) {
        double s = sin((double)k);
        if (s == 0.0)  /* sin(k) is (practically) zero: skip to avoid inf. */
            continue;

        double val = floor((double)p / s);
        /* Since floor returns a double, cast to uint32_t for comparison. */
        if ((uint32_t)val == q)
            return (seq_t)k;   /* found the smallest k */
    }
}
