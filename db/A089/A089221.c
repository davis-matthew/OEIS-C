/*
 * A089221  a(n) = floor(sqrt(((prime(n) + primepi(n))/2)^2 +
 *          prime(n)*primepi(n))).
 * https://oeis.org/A089221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(sqrt(((prime(n) + primepi(n))/2)^2 +
 *            prime(n)*primepi(n)))
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   compute_pi, floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where primepi(n) = π(n) = number of primes ≤ n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n = 0, return 0 for safety */
        return 0;

    /* nth prime and the prime counting function at n. */
    const seq_t pn   = get_prime(n);   /* prime(n)   */
    const seq_t pi_n = compute_pi(n);  /* primepi(n) = π(n) */

    /* s = p + π(n) */
    const seq_t s = pn + pi_n;

    /* v = (p + π)^2 + 4 * p * π  */
    const seq_t v = s * s + (seq_t)4 * pn * pi_n;

    /* floor(sqrt(v)) gives floor( sqrt( ((p+π)^2)/4 + pπ ) * 2 ) */
    const seq_t sqrtv = floor_sqrt(v);

    /* The desired value is floor(sqrt(v)) / 2. */
    return sqrtv / 2;
}
