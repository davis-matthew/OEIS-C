/*
 * A234541  Least k such that floor(n/k) + (n mod k) is a prime, or 0 if no
 *          such k exists.
 * https://oeis.org/A234541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest k such that floor(n/k)+(n%k) is prime, or 0 if none */

/* Prime testing helper */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined for n >= 1, but guard against 0 */
        return 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute q = n / k and r = n % k */
        idx_t q = n / k;
        idx_t r = n % k;
        idx_t s = q + r;     /* sum is always <= n, fits in idx_t */

        if (is_prime(s))
            return (seq_t)k;   /* found the smallest suitable k */
    }
    return 0;  /* no such k */
}
